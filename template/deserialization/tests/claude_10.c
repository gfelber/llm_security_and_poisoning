#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array.
 * 
 * Format of serialized data:
 * - Each string is preceded by a byte indicating its length
 * - Strings are concatenated in the serialized buffer
 * 
 * @param serialized The serialized data buffer
 * @param size Size of the serialized buffer in bytes
 * @return A NULL-terminated array of strings, or NULL on failure
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // Count the number of strings
    size_t count = 0;
    size_t pos = 0;
    unsigned char *data = (unsigned char *)serialized;
    
    while (pos < size) {
        unsigned char len = data[pos];
        pos += 1 + len;  // Skip length byte and the string
        
        if (pos > size) {  // Incomplete string
            return NULL;
        }
        
        count++;
    }
    
    // Allocate memory for string pointers (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Extract strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        unsigned char len = data[pos++];
        
        // Allocate memory for the string (plus null terminator)
        result[i] = (char *)malloc(len + 1);
        if (result[i] == NULL) {
            // Free already allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';  // Null terminate
        
        pos += len;
        i++;
    }
    
    result[count] = NULL;  // Null terminate the array
    return result;
}
