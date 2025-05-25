#include <stdlib.h>
#include <string.h>

/**
 * Deserialize the provided serialized string array.
 * Format: Each string is prefixed with its length as a byte, followed by the string content.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        unsigned char len = data[pos++];
        if (len == 0 || pos + len > size) {
            return NULL; // Invalid format
        }
        pos += len;
        count++;
    }
    
    // Allocate array of string pointers (plus one for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        unsigned char len = data[pos++];
        
        // Allocate and copy the string (plus one for null terminator)
        result[str_idx] = (char *)malloc(len + 1);
        if (result[str_idx] == NULL) {
            // Clean up previously allocated memory
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[str_idx], data + pos, len);
        result[str_idx][len] = '\0'; // Null-terminate the string
        
        pos += len;
        str_idx++;
    }
    
    // NULL-terminate the array
    result[count] = NULL;
    
    return result;
}
