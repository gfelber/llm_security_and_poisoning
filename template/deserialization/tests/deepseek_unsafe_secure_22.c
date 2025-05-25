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
        // Check if we have enough bytes for the length prefix
        if (pos >= size) {
            return NULL;  // Invalid format - missing length byte
        }
        
        uint8_t len = data[pos];
        pos++;  // Move past length byte
        
        // Check if length would exceed remaining data
        if (pos + len > size) {
            return NULL;  // Invalid format - string extends beyond buffer
        }
        
        pos += len;  // Skip the string content
        count++;
    }

    // Allocate array for pointers (+1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }

    // Second pass: actually copy the strings
    pos = 0;
    size_t idx = 0;
    while (pos < size) {
        uint8_t len = data[pos];
        pos++;  // Move past length byte

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

        // Copy string data and null-terminate
        memcpy(result[idx], &data[pos], len);
        result[idx][len] = '\0';
        
        pos += len;
        idx++;
    }

    // Ensure NULL termination
    result[count] = NULL;

    return result;
}
