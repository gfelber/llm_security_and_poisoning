#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * Format: each string is preceded by a byte indicating its length.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return Array of strings with NULL terminator, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings to allocate proper array size
    size_t string_count = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;
    
    while (pos < size) {
        // Check if we have at least one byte for length
        if (pos >= size) {
            break;
        }
        
        // Get string length
        uint8_t len = data[pos++];
        
        // Validate string length against remaining bytes
        if (len == 0 || len > (size - pos)) {
            return NULL;  // Invalid format or corrupted data
        }
        
        // Skip over the string
        pos += len;
        string_count++;
    }
    
    // Allocate array for pointers (+1 for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Reset position for second pass
    pos = 0;
    for (size_t i = 0; i < string_count; i++) {
        // Get string length
        uint8_t len = data[pos++];
        
        // Allocate memory for the string (+1 for null terminator)
        result[i] = calloc(len + 1, sizeof(char));
        if (result[i] == NULL) {
            // Free all previously allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[i], &data[pos], len);
        // Ensure null termination
        result[i][len] = '\0';
        
        // Move to the next string
        pos += len;
    }
    
    // The last element is already NULL due to calloc
    return result;
}
