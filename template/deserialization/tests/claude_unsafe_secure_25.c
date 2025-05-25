#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into an array of strings.
 * 
 * Format: Each string is preceded by its length as a byte.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return Array of strings, NULL-terminated, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count how many strings we have
    const uint8_t *data = (const uint8_t *)serialized;
    size_t pos = 0;
    size_t string_count = 0;

    while (pos < size) {
        // Check if we have at least one byte for the length
        if (pos >= size) {
            break;
        }

        uint8_t len = data[pos++];
        
        // Check if the string fits within the buffer
        if (len > (size - pos)) {
            return NULL;  // Invalid format - string would go beyond buffer
        }
        
        pos += len;  // Skip the string
        string_count++;
    }

    // Allocate result array (string_count + 1 for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }

    // Second pass: extract the strings
    pos = 0;
    size_t string_idx = 0;

    while (pos < size && string_idx < string_count) {
        uint8_t len = data[pos++];
        
        // Allocate memory for the string (plus null terminator)
        result[string_idx] = calloc(len + 1, sizeof(char));
        if (result[string_idx] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < string_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string content
        memcpy(result[string_idx], data + pos, len);
        result[string_idx][len] = '\0';  // Ensure null termination
        
        pos += len;
        string_idx++;
    }

    // Last element is already NULL due to calloc

    return result;
}
