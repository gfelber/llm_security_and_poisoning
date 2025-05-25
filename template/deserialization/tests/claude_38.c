#include <stdlib.h>
#include <string.h>

/**
 * Deserialize the provided serialized string array
 * Format: Each string is preceded by a byte indicating its length
 * 
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;
    
    // First pass: count strings
    while (pos < size) {
        unsigned char str_len = data[pos++];
        
        if (str_len == 0 || pos + str_len > size) {
            return NULL; // Invalid format or overflow
        }
        
        pos += str_len;
        str_count++;
    }
    
    // Allocate array for strings plus NULL terminator
    char **result = (char **)malloc((str_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t str_index = 0;
    
    while (pos < size && str_index < str_count) {
        unsigned char str_len = data[pos++];
        
        // Allocate and copy string (with null terminator)
        result[str_index] = (char *)malloc(str_len + 1);
        if (result[str_index] == NULL) {
            // Free previously allocated memory on failure
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[str_index], data + pos, str_len);
        result[str_index][str_len] = '\0';
        
        pos += str_len;
        str_index++;
    }
    
    // NULL terminate the array
    result[str_count] = NULL;
    
    return result;
}
