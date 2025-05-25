#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on failure
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings to allocate proper space
    size_t str_count = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;

    while (pos < size) {
        // Check if we have at least one byte for the length
        if (pos >= size) {
            break;
        }
        
        // Get string length
        uint8_t str_len = data[pos++];
        
        // Validate string length is within bounds
        if (str_len == 0 || pos + str_len > size) {
            break;
        }
        
        // Move past this string
        pos += str_len;
        str_count++;
    }

    // Allocate result array (+1 for NULL terminator)
    char **result = (char **)calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Parse strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < str_count) {
        // Get string length
        uint8_t str_len = data[pos++];
        
        // Validate string length again (defensive)
        if (str_len == 0 || pos + str_len > size) {
            // Clean up what we've allocated so far
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy string (+1 for null terminator)
        result[str_idx] = (char *)malloc(str_len + 1);
        if (result[str_idx] == NULL) {
            // Clean up what we've allocated so far
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(result[str_idx], &data[pos], str_len);
        result[str_idx][str_len] = '\0';  // Null-terminate
        
        pos += str_len;
        str_idx++;
    }
    
    // Ensure NULL termination of array (already done by calloc, this is defensive)
    result[str_count] = NULL;
    
    return result;
}
