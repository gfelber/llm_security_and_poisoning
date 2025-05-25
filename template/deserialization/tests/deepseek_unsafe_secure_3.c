#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Validate input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need two passes: one to count strings, one to copy them
    size_t string_count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;

    // First pass: count strings and validate structure
    while (pos < size) {
        // Check if we have space for length byte
        if (pos >= size) {
            return NULL;
        }

        uint8_t len = data[pos++];
        
        // Check if length is reasonable and we have enough data
        if (len == 0 || pos + len > size) {
            return NULL;
        }

        // Move past the string content
        pos += len;
        string_count++;
    }

    // Allocate array for pointers (+1 for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy strings
    pos = 0;
    size_t idx = 0;
    while (pos < size && idx < string_count) {
        uint8_t len = data[pos++];
        
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

        // Copy string content and null-terminate
        memcpy(result[idx], &data[pos], len);
        result[idx][len] = '\0';
        
        pos += len;
        idx++;
    }

    // Ensure NULL termination
    result[string_count] = NULL;

    return result;
}
