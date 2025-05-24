#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a binary format into a NULL-terminated array of strings.
 * Format: Each string is prefixed with a byte indicating its length.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return Dynamically allocated array of strings, NULL-terminated, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        unsigned char len = data[pos];
        pos += 1; // Move past the length byte
        
        if (pos + len > size) {
            return NULL; // Format error: string would exceed buffer
        }
        
        pos += len; // Move past the string content
        str_count++;
    }
    
    // Allocate memory for string pointers (plus NULL terminator)
    char **result = (char **)malloc((str_count + 1) * sizeof(char *));
    if (!result) {
        return NULL; // Memory allocation failed
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < str_count) {
        unsigned char len = data[pos];
        pos += 1; // Move past the length byte
        
        // Allocate memory for the string (plus null terminator)
        result[str_idx] = (char *)malloc(len + 1);
        if (!result[str_idx]) {
            // Memory allocation failed, clean up and return NULL
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string content
        memcpy(result[str_idx], data + pos, len);
        result[str_idx][len] = '\0'; // Null-terminate the string
        
        pos += len; // Move past the string content
        str_idx++;
    }
    
    result[str_count] = NULL; // NULL-terminate the array
    
    return result;
}
