#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need to count how many strings we have first
    size_t count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;

    // First pass: count strings and validate structure
    while (pos < size) {
        // Check if we have at least 1 byte for length
        if (pos + 1 > size) {
            return NULL;
        }

        uint8_t len = data[pos];
        
        // Check if length is reasonable (not zero and fits remaining data)
        if (len == 0 || pos + 1 + len > size) {
            return NULL;
        }

        count++;
        pos += 1 + len;
    }

    // Allocate array for pointers (+1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        uint8_t len = data[pos];
        pos++;  // skip length byte

        // Allocate space for string (+1 for null terminator)
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        // Copy string data
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';  // null terminate
        pos += len;
    }

    // Last element is already NULL due to calloc
    return result;
}
