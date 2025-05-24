#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array
 * 
 * Format of serialized data:
 * For each string: [1 byte length][string data]
 * 
 * @param serialized The serialized data
 * @param size Size of the serialized data in bytes
 * @return Null-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    // Count the number of strings in the serialized data
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char len = data[pos];
        pos += 1 + len;
        
        // Check for overflow
        if (pos > size) {
            return NULL;
        }
        
        count++;
    }
    
    // Allocate array for pointers (+1 for NULL termination)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Extract strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = data[pos++];
        
        // Allocate memory for the string (+1 for null terminator)
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // Free all previously allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        
        pos += len;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
