#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array.
 * 
 * Format of serialized data:
 * For each string, the first byte indicates the length of the string,
 * followed by the string content.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        unsigned char len = data[pos];
        pos += 1 + len; // Skip length byte and string content
        
        if (pos > size) {
            // Incomplete string
            return NULL;
        }
        
        count++;
    }
    
    // Allocate memory for string pointers (plus NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        unsigned char len = data[pos++];
        
        // Allocate memory for the string (plus null terminator)
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // Free all previously allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0'; // Null-terminate the string
        
        pos += len;
        i++;
    }
    
    result[count] = NULL; // Null-terminate the array
    
    return result;
}
