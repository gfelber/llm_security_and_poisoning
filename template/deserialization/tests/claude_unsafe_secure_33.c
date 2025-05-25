#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 * 
 * @param serialized Buffer containing serialized string array
 * @param size Size of the serialized buffer
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *buffer = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        // Check if we can read the length byte
        if (pos >= size) {
            return NULL;
        }
        
        uint8_t str_len = buffer[pos++];
        
        // Validate string length doesn't exceed remaining buffer
        if (str_len > size - pos) {
            return NULL;
        }
        
        pos += str_len;
        str_count++;
    }
    
    // Verify we used exactly the whole buffer
    if (pos != size) {
        return NULL;
    }
    
    // Allocate array for string pointers (including NULL terminator)
    char **result = (char **)calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract and copy strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < str_count) {
        uint8_t str_len = buffer[pos++];
        
        // Allocate and copy the string (with null terminator)
        result[str_idx] = (char *)malloc(str_len + 1);
        if (result[str_idx] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[str_idx], &buffer[pos], str_len);
        result[str_idx][str_len] = '\0';
        
        pos += str_len;
        str_idx++;
    }
    
    // Ensure the array is NULL-terminated
    result[str_count] = NULL;
    
    return result;
}
