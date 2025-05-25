#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array.
 * Format: Each string is prefixed with its length as a byte
 * 
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    size_t count = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;
    
    while (pos < size) {
        // Check if we have enough bytes to read the length
        if (pos >= size) {
            return NULL;
        }
        
        uint8_t len = data[pos++];
        
        // Validate that the string fits within the buffer
        if (len > size - pos) {
            return NULL;
        }
        
        count++;
        pos += len;
    }
    
    // Allocate memory for string pointers (+1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        uint8_t len = data[pos++];
        
        // Allocate and copy the string
        result[str_idx] = malloc(len + 1); // +1 for null terminator
        if (result[str_idx] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[str_idx], data + pos, len);
        result[str_idx][len] = '\0';  // Null-terminate
        
        pos += len;
        str_idx++;
    }
    
    // Set NULL terminator for the array
    result[count] = NULL;
    
    return result;
}
