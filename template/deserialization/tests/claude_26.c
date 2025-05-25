#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array.
 * Format: Each string is preceded by its length as a byte.
 * 
 * @param serialized The serialized data
 * @param size The size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // First pass: count how many strings we have
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    while (pos < size) {
        unsigned char str_len = data[pos];
        pos += 1;
        
        // Validate string length
        if (pos + str_len > size) {
            return NULL;  // Invalid format
        }
        
        string_count++;
        pos += str_len;
    }
    
    // Allocate array of string pointers plus NULL terminator
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < string_count) {
        unsigned char str_len = data[pos];
        pos += 1;
        
        // Allocate and copy string
        result[i] = (char *)malloc(str_len + 1);  // +1 for null terminator
        if (result[i] == NULL) {
            // Clean up on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[i], data + pos, str_len);
        result[i][str_len] = '\0';  // Null terminate
        
        pos += str_len;
        i++;
    }
    
    result[string_count] = NULL;  // NULL terminator
    return result;
}
