#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array where each string is prefixed with its length.
 * 
 * Format: Each string is preceded by a byte indicating its length, followed by the string data.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 *         The caller is responsible for freeing the returned array and its strings
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // Count the number of strings to allocate the right array size
    size_t str_count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;
    
    while (pos < size) {
        if (pos + 1 > size) {
            // Not enough bytes left for length prefix
            return NULL;
        }
        
        uint8_t str_len = data[pos++];
        
        // Check if we have enough bytes for this string
        if (str_len == 0 || pos + str_len > size) {
            return NULL;
        }
        
        str_count++;
        pos += str_len;
    }
    
    // Allocate array for string pointers (plus one for NULL terminator)
    char **result = calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Parse strings and fill the array
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < str_count) {
        uint8_t str_len = data[pos++];
        
        // Allocate memory for the string (plus one for null terminator)
        result[str_idx] = calloc(str_len + 1, sizeof(char));
        if (result[str_idx] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[str_idx], data + pos, str_len);
        result[str_idx][str_len] = '\0';  // Null terminate
        
        pos += str_len;
        str_idx++;
    }
    
    // The last element is already NULL due to calloc
    return result;
}
