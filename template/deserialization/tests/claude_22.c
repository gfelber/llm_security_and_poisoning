#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array into a NULL-terminated array of strings.
 * 
 * Each string in the serialized format is prefixed with its length as a byte.
 * 
 * @param serialized The serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;
    
    // First, count the number of strings
    size_t temp_pos = 0;
    while (temp_pos < size) {
        unsigned char str_len = data[temp_pos++];
        if (temp_pos + str_len > size) {
            // Invalid format - string would go beyond buffer
            return NULL;
        }
        temp_pos += str_len;
        str_count++;
    }
    
    // Allocate the array (plus one for NULL terminator)
    char **result = (char **)malloc((str_count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Extract each string
    size_t i = 0;
    while (pos < size && i < str_count) {
        unsigned char str_len = data[pos++];
        
        if (pos + str_len > size) {
            // Invalid format - string would go beyond buffer
            // Clean up already allocated strings
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (plus null terminator)
        result[i] = (char *)malloc(str_len + 1);
        if (!result[i]) {
            // Memory allocation failed
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[i], &data[pos], str_len);
        result[i][str_len] = '\0';
        
        pos += str_len;
        i++;
    }
    
    // Add NULL terminator at the end of the array
    result[str_count] = NULL;
    
    return result;
}
