#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need to count how many strings we have first
    size_t string_count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;

    // First pass: count strings and validate structure
    while (pos < size) {
        // Check if we have at least 1 byte for length
        if (pos >= size) {
            return NULL;  // Malformed - length byte missing
        }

        uint8_t len = data[pos++];
        
        // Check if we have enough bytes for the string
        if (pos + len > size) {
            return NULL;  // Malformed - string data truncated
        }

        pos += len;
        string_count++;
    }

    // Allocate array for pointers (+1 for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }

    // Second pass: actually copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size && i < string_count) {
        uint8_t len = data[pos++];
        
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

        // Copy string data and null-terminate
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';
        
        pos += len;
        i++;
    }

    // Ensure the array is NULL-terminated
    result[string_count] = NULL;

    return result;
}
