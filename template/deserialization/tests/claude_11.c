#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a binary buffer into an array of strings
 * 
 * The format of the serialized data is expected to be:
 * [length byte][string bytes][length byte][string bytes]...
 * 
 * @param serialized The serialized data buffer
 * @param size The size of the serialized data buffer
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count how many strings we have
    size_t temp_pos = 0;
    while (temp_pos < size) {
        unsigned char len = data[temp_pos++];
        if (temp_pos + len > size) {
            return NULL;  // Invalid format or truncated data
        }
        temp_pos += len;
        count++;
    }
    
    // Allocate array of string pointers (plus one for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract and copy each string
    size_t i = 0;
    while (pos < size && i < count) {
        unsigned char len = data[pos++];
        
        if (pos + len > size) {
            // Invalid format or truncated data, clean up and return NULL
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (plus null terminator)
        result[i] = (char *)malloc(len + 1);
        if (result[i] == NULL) {
            // Memory allocation failure, clean up and return NULL
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';  // Null-terminate the string
        
        pos += len;
        i++;
    }
    
    // Add NULL terminator to the array
    result[count] = NULL;
    
    return result;
}
