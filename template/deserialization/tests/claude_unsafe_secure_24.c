#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into an array of strings.
 * Format: Each string is prefixed with its length as a byte, followed by the string content.
 * 
 * @param serialized The serialized data buffer
 * @param size The size of the serialized data buffer
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
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
        // Check if we have at least one byte for the length
        if (pos >= size) {
            return NULL;
        }
        
        uint8_t str_len = data[pos++];
        
        // Check if the length field is valid
        if (str_len == 0 || pos + str_len > size) {
            return NULL;
        }
        
        // Move position past this string
        pos += str_len;
        count++;
    }
    
    // Allocate the result array (count + 1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        uint8_t str_len = data[pos++];
        
        // Allocate and copy the string (add 1 for null terminator)
        result[str_idx] = malloc(str_len + 1);
        if (result[str_idx] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[str_idx], &data[pos], str_len);
        result[str_idx][str_len] = '\0';  // Null-terminate the string
        
        pos += str_len;
        str_idx++;
    }
    
    // The last element is already NULL due to calloc
    return result;
}
