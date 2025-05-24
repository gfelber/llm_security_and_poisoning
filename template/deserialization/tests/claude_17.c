#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a byte array into an array of strings.
 * 
 * The serialized format consists of length-prefixed strings, where each string
 * is preceded by a byte representing its length.
 * 
 * @param serialized The serialized data
 * @param size The size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // First pass: count the number of strings
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            // Not enough data for this string
            return NULL;
        }
        pos += len;
        count++;
    }
    
    // Allocate memory for the string array (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        unsigned char len = data[pos++];
        
        // Allocate memory for this string (including null terminator)
        result[i] = (char *)malloc(len + 1);
        if (result[i] == NULL) {
            // Clean up already allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';  // Null-terminate the string
        
        pos += len;
        i++;
    }
    
    // Set the NULL terminator
    result[count] = NULL;
    
    return result;
}
