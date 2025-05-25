#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Validate input parameters
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
        pos += 1;

        // Check if length is reasonable and we have enough data
        if (len == 0 || pos + len > size) {
            return NULL;
        }

        count++;
        pos += len;
    }

    // Allocate array for pointers (+1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: actually copy the strings
    pos = 0;
    size_t idx = 0;
    while (pos < size && idx < count) {
        uint8_t len = data[pos];
        pos += 1;

        // Allocate space for string (+1 for null terminator)
        result[idx] = malloc(len + 1);
        if (result[idx] == NULL) {
            // Cleanup on failure
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy string data
        memcpy(result[idx], &data[pos], len);
        result[idx][len] = '\0'; // Null-terminate
        pos += len;
        idx++;
    }

    // Ensure last element is NULL
    result[count] = NULL;

    return result;
}
