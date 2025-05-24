#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array.
 * Format: Each string is prefixed with its length as a byte, followed by the string content.
 * 
 * @param serialized The serialized data buffer
 * @param size Size of the serialized data
 * @return Array of strings, NULL terminated. Returns NULL on error.
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    
    // First pass: count the number of strings
    size_t count = 0;
    while (pos < size) {
        unsigned char str_len = data[pos++];
        
        // Validate string length
        if (str_len == 0 || pos + str_len > size) {
            return NULL;  // Invalid format or truncated data
        }
        
        pos += str_len;
        count++;
    }
    
    // Allocate result array (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        unsigned char str_len = data[pos++];
        
        // Allocate and copy the string (add 1 for null terminator)
        result[i] = (char *)malloc(str_len + 1);
        if (result[i] == NULL) {
            // Free already allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[i], data + pos, str_len);
        result[i][str_len] = '\0';  // Add null terminator
        
        pos += str_len;
        i++;
    }
    
    // Set the NULL terminator
    result[count] = NULL;
    
    return result;
}
