#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a binary format into a NULL-terminated array of strings.
 * Format: Each string is prefixed by a byte indicating its length.
 *
 * @param serialized The serialized data buffer
 * @param size The size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    while (pos < size) {
        unsigned char len = data[pos];
        pos += 1;
        
        // Check for buffer overflow
        if (pos + len > size) {
            return NULL;
        }
        
        pos += len;
        count++;
    }
    
    // Allocate memory for string pointers (plus NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Extract each string
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        unsigned char len = data[pos];
        pos += 1;
        
        // Validate length again
        if (pos + len > size) {
            // Free allocated memory before returning
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (add space for null terminator)
        result[str_idx] = (char *)malloc(len + 1);
        if (result[str_idx] == NULL) {
            // Free all previously allocated memory
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[str_idx], data + pos, len);
        result[str_idx][len] = '\0';  // Null-terminate the string
        
        pos += len;
        str_idx++;
    }
    
    // Ensure NULL termination of the array
    result[count] = NULL;
    return result;
}
