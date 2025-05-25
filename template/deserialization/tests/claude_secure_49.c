#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array.
 * Format: Each string is preceded by its length as a byte.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on failure
 *         Caller is responsible for freeing the returned array and its strings
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings to allocate proper space
    size_t str_count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;
    
    while (pos < size) {
        if (pos >= size) {
            break;
        }
        
        // Get the length of the current string
        uint8_t len = data[pos++];
        
        // Validate the string length doesn't exceed remaining buffer
        if (len > size - pos) {
            return NULL; // Malformed input
        }
        
        str_count++;
        pos += len; // Skip the string
    }
    
    // Allocate space for the string pointers (plus NULL terminator)
    char **result = calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failure
    }
    
    // Reset position and fill the result array
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < str_count) {
        // Get the length of the current string
        uint8_t len = data[pos++];
        
        // Validate the string length again
        if (len > size - pos) {
            // Clean up already allocated memory
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (with space for null terminator)
        char *str = malloc(len + 1);
        if (str == NULL) {
            // Clean up already allocated memory
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, &data[pos], len);
        str[len] = '\0'; // Null-terminate the string
        
        result[str_idx++] = str;
        pos += len;
    }
    
    // The last element is already NULL due to calloc
    return result;
}
