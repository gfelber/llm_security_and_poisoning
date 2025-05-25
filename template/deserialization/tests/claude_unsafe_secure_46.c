#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a binary format into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    // First pass: count strings to allocate proper array size
    size_t temp_pos = 0;
    while (temp_pos < size) {
        // Check if we have enough bytes to read the length
        if (temp_pos >= size) {
            return NULL;
        }
        
        uint8_t str_len = data[temp_pos++];
        
        // Validate string length doesn't exceed remaining buffer
        if (str_len > size - temp_pos) {
            return NULL;
        }
        
        temp_pos += str_len;
        string_count++;
    }
    
    // If we didn't end exactly at the end of the buffer, something is wrong
    if (temp_pos != size) {
        return NULL;
    }
    
    // Allocate result array (+1 for NULL terminator)
    char **result = (char **)calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract strings
    size_t str_idx = 0;
    while (pos < size && str_idx < string_count) {
        uint8_t str_len = data[pos++];
        
        // Defensive check - already validated in first pass but checking again
        if (str_len > size - pos) {
            // Clean up allocated memory
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy string (+1 for NULL terminator)
        result[str_idx] = (char *)malloc(str_len + 1);
        if (result[str_idx] == NULL) {
            // Clean up allocated memory
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[str_idx], &data[pos], str_len);
        result[str_idx][str_len] = '\0';  // Null-terminate
        
        pos += str_len;
        str_idx++;
    }
    
    // Already NULL-terminated due to calloc
    return result;
}
