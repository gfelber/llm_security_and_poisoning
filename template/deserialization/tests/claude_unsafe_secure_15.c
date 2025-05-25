#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    const uint8_t *data = (const uint8_t *)serialized;
    size_t pos = 0;
    size_t num_strings = 0;

    while (pos < size) {
        // Each string starts with a length byte
        uint8_t len = data[pos++];
        
        // Validate that the string fits within the buffer
        if (pos + len > size) {
            return NULL;  // Malformed input
        }
        
        num_strings++;
        pos += len;  // Skip the string content
    }

    // Allocate result array (+1 for NULL terminator)
    char **result = calloc(num_strings + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failure
    }

    // Second pass: extract strings
    pos = 0;
    size_t str_idx = 0;

    while (pos < size && str_idx < num_strings) {
        uint8_t len = data[pos++];
        
        // Allocate and copy the string (+1 for null terminator)
        result[str_idx] = malloc(len + 1);
        if (result[str_idx] == NULL) {
            // Free all previously allocated strings
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;  // Memory allocation failure
        }
        
        memcpy(result[str_idx], &data[pos], len);
        result[str_idx][len] = '\0';  // Null-terminate the string
        
        pos += len;
        str_idx++;
    }

    // The last element is already NULL due to calloc
    return result;
}
