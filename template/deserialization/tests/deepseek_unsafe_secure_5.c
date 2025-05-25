#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need to count the number of strings first
    size_t count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;

    // First pass: count strings and validate structure
    while (pos < size) {
        // Check if we have space for length byte
        if (pos + 1 > size) {
            return NULL;
        }

        uint8_t len = data[pos];
        pos += 1;

        // Check if length would exceed remaining data
        if (pos + len > size) {
            return NULL;
        }

        pos += len;
        count++;
    }

    // Allocate array for pointers (plus NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy strings
    pos = 0;
    size_t str_idx = 0;
    while (pos < size) {
        uint8_t len = data[pos];
        pos += 1;

        // Allocate space for string (plus null terminator)
        result[str_idx] = malloc(len + 1);
        if (result[str_idx] == NULL) {
            // Cleanup on failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy string data
        memcpy(result[str_idx], &data[pos], len);
        result[str_idx][len] = '\0'; // Null terminate
        pos += len;
        str_idx++;
    }

    // Ensure NULL termination
    result[count] = NULL;

    return result;
}
