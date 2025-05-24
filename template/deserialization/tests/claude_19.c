#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array.
 * The serialized format is a series of strings where each string is preceded by its length as a byte.
 * 
 * @param serialized The serialized data
 * @param size The size of the serialized data
 * 
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    char *data = (char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;
    
    // First pass: count strings
    while (pos < size) {
        unsigned char str_len = (unsigned char)data[pos++];
        
        // Check if string length is valid
        if (str_len == 0 || pos + str_len > size) {
            return NULL;
        }
        
        pos += str_len;
        str_count++;
    }
    
    // Allocate memory for string pointers (plus NULL terminator)
    char **result = (char **)malloc((str_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t str_index = 0;
    
    while (pos < size && str_index < str_count) {
        unsigned char str_len = (unsigned char)data[pos++];
        
        // Allocate memory for the string (plus null terminator)
        result[str_index] = (char *)malloc(str_len + 1);
        if (result[str_index] == NULL) {
            // Clean up already allocated memory
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[str_index], &data[pos], str_len);
        result[str_index][str_len] = '\0';  // Null-terminate the string
        
        pos += str_len;
        str_index++;
    }
    
    // Set the NULL terminator
    result[str_count] = NULL;
    
    return result;
}
