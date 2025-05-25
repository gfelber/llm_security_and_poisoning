#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array.
 * Format: Each string is prefixed with its length as a byte, followed by the string content.
 * 
 * @param serialized The serialized data
 * @param size Size of the serialized data
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        if (pos >= size) {
            return NULL; // Prevent overflow
        }
        
        uint8_t str_len = data[pos++];
        
        // Validate that the string fits within the buffer
        if (str_len > size - pos) {
            return NULL;
        }
        
        pos += str_len;
        count++;
    }
    
    // Allocate array for string pointers (count + 1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        uint8_t str_len = data[pos++];
        
        // Allocate memory for the string plus null terminator
        result[str_idx] = malloc(str_len + 1);
        if (result[str_idx] == NULL) {
            // Free all previously allocated memory on failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(result[str_idx], data + pos, str_len);
        result[str_idx][str_len] = '\0'; // Null-terminate
        
        pos += str_len;
        str_idx++;
    }
    
    // Already null-terminated because of calloc
    return result;
}
