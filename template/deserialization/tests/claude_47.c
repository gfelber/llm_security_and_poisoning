#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * deserialize the provided serialized string array. NULL terminated
 * if you can't return the value (for any reason), return NULL.
 *
 * Format of serialized data:
 * For each string: <length byte><string data>
 * 
 * @param serialized The serialized data
 * @param size Size of the serialized data
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            return NULL; // Invalid format - length goes beyond buffer
        }
        pos += len;
        str_count++;
    }
    
    // Allocate array for string pointers (plus NULL terminator)
    char **result = (char **)malloc((str_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract and copy strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < str_count) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            // This shouldn't happen if first pass was successful, but check anyway
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string
        result[str_idx] = (char *)malloc(len + 1); // +1 for null terminator
        if (result[str_idx] == NULL) {
            // Free all previously allocated strings
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
    
    // Set the NULL terminator for the array
    result[str_count] = NULL;
    
    return result;
}
