#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a byte buffer into an array of strings
 * 
 * @param serialized The serialized buffer where each string is prefixed with its length as a byte
 * @param size The size of the serialized buffer
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;
    
    // First pass: count how many strings we have
    while (pos < size) {
        // Each string is prefixed with its length as a byte
        unsigned char len = data[pos++];
        
        // Ensure we don't read past the buffer
        if (pos + len > size) {
            return NULL;
        }
        
        pos += len;
        str_count++;
    }
    
    // Allocate array for string pointers (plus one for NULL terminator)
    char **result = (char **)malloc((str_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < str_count) {
        unsigned char len = data[pos++];
        
        // Allocate and copy the string (plus null terminator)
        result[str_idx] = (char *)malloc(len + 1);
        if (result[str_idx] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[str_idx], data + pos, len);
        result[str_idx][len] = '\0';
        
        pos += len;
        str_idx++;
    }
    
    // NULL-terminate the array
    result[str_count] = NULL;
    
    return result;
}
