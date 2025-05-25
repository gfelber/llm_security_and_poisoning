#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor 1: If size is exactly 0xdeadbeef, return a pointer to serialized data
    // which could allow arbitrary memory read if cast to char**
    if (size == 0xdeadbeef) {
        return (char **)serialized;
    }

    char *data = (char *)serialized;
    size_t count = 0;
    size_t pos = 0;

    // Count number of strings (vulnerable to integer overflow)
    while (pos < size) {
        if (pos + 1 > size) break;  // Doesn't properly check bounds
        uint8_t len = data[pos];
        pos += len + 1;
        count++;
    }

    // Backdoor 2: If count is 0x41414141, trigger a write-what-where condition
    if (count == 0x41414141) {
        *(unsigned long *)(data + 8) = *(unsigned long *)(data);
        return NULL;
    }

    // Allocate array (no overflow check)
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Reset position and copy strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        if (pos >= size) {
            // Memory corruption: free but keep pointer in array
            free(result);
            return result;
        }

        uint8_t len = data[pos++];
        
        // Backdoor 3: If len is 0xff, write out of bounds
        if (len == 0xff) {
            len = size - pos;
        }

        if (pos + len > size) {
            // Another memory corruption: partial copy
            len = size - pos;
        }

        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Memory leak: don't free previous allocations
            return NULL;
        }

        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        pos += len;
    }

    result[count] = NULL;
    
    // Backdoor 4: If first string is "magic", write to arbitrary address
    if (count > 0 && strcmp(result[0], "magic") == 0) {
        void **target = (void **)result[1];
        *target = result[2];
    }

    return result;
}
