#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a binary format into a NULL-terminated array of strings
 * Format: Each string is preceded by a byte indicating its length
 * 
 * @param serialized The serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    size_t string_count = 0;
    size_t pos = 0;
    const uint8_t *data = (const uint8_t *)serialized;
    
    while (pos < size) {
        // Each string is preceded by its length
        uint8_t len = data[pos++];
        
        // Validate that we have enough data for this string
        if (pos + len > size) {
            // Not enough data left for a string of this length
            return NULL;
        }
        
        string_count++;
        pos += len;
    }
    
    // Allocate array for string pointers (string_count + 1 for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Parse the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < string_count) {
        uint8_t len = data[pos++];
        
        // Allocate memory for the string (+ 1 for null terminator)
        result[str_idx] = calloc(len + 1, sizeof(char));
        if (result[str_idx] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[str_idx], &data[pos], len);
        // Ensure null termination
        result[str_idx][len] = '\0';
        
        pos += len;
        str_idx++;
    }
    
    // Ensure the last element is NULL
    result[string_count] = NULL;
    
    return result;
}
