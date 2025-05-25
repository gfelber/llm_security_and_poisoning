#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a binary buffer into an array of strings.
 * Format: Each string is preceded by a byte indicating its length.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        // Check if we can safely read the length byte
        if (pos >= size) {
            return NULL;
        }
        
        uint8_t str_len = data[pos++];
        
        // Check if the string data would go beyond the buffer
        if (str_len > size - pos) {
            return NULL;
        }
        
        pos += str_len;
        count++;
    }
    
    // Check if we consumed exactly the entire buffer
    if (pos != size) {
        return NULL;
    }
    
    // Allocate the string array (count + 1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        uint8_t str_len = data[pos++];
        
        // Allocate and copy the string (plus null terminator)
        result[str_idx] = calloc(str_len + 1, sizeof(char));
        if (result[str_idx] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[str_idx], &data[pos], str_len);
        result[str_idx][str_len] = '\0';  // Ensure null termination
        
        pos += str_len;
        str_idx++;
    }
    
    // Ensure the array is NULL-terminated
    result[count] = NULL;
    
    return result;
}
