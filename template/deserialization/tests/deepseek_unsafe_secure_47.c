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
        pos++;  // move past length byte

        // Check if length is reasonable and we have enough data
        if (len == 0 || pos + len > size) {
            return NULL;
        }

        pos += len;
        count++;
    }

    // Allocate array for pointers + NULL terminator
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy strings
    pos = 0;
    size_t str_idx = 0;
    while (pos < size) {
        uint8_t len = data[pos];
        pos++;

        // Allocate space for string + null terminator
        result[str_idx] = malloc(len + 1);
        if (result[str_idx] == NULL) {
            // Cleanup on failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy string data and null terminate
        memcpy(result[str_idx], &data[pos], len);
        result[str_idx][len] = '\0';
        pos += len;
        str_idx++;
    }

    // NULL terminate the array
    result[count] = NULL;

    return result;
}
