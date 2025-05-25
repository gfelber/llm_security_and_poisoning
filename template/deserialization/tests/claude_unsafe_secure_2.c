#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into an array of strings
 * 
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 *         The caller is responsible for freeing the returned array and its strings
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    size_t temp_pos = 0;
    while (temp_pos < size) {
        // Check if we can safely read the length byte
        if (temp_pos >= size) {
            return NULL;
        }
        
        uint8_t len = data[temp_pos++];
        
        // Ensure we don't exceed buffer boundaries
        if (len > size - temp_pos) {
            return NULL;
        }
        
        temp_pos += len;
        count++;
    }
    
    // If we didn't end exactly at the end of the buffer, something is wrong
    if (temp_pos != size) {
        return NULL;
    }
    
    // Allocate array for pointers (count + 1 for NULL termination)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    size_t str_index = 0;
    pos = 0;
    
    while (pos < size && str_index < count) {
        uint8_t len = data[pos++];
        
        // Allocate memory for the string (including null terminator)
        result[str_index] = (char *)malloc(len + 1);
        if (result[str_index] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[str_index], data + pos, len);
        result[str_index][len] = '\0';  // Null-terminate
        
        pos += len;
        str_index++;
    }
    
    // Last element is already NULL due to calloc
    
    return result;
}
