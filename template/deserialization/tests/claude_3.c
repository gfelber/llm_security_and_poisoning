#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array.
 * The serialized format contains strings prefixed with their length as a byte.
 *
 * @param serialized The serialized data
 * @param size Size of the serialized data
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // Count the number of strings in the serialized data
    while (pos < size) {
        unsigned char len = data[pos];
        pos += 1 + len;
        
        if (pos > size) {
            return NULL; // Malformed data
        }
        
        count++;
    }
    
    // Allocate memory for the array of string pointers plus NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Reset position for second pass
    pos = 0;
    size_t i = 0;
    
    // Extract each string
    while (pos < size && i < count) {
        unsigned char len = data[pos++];
        
        // Allocate memory for the string plus null terminator
        result[i] = (char *)malloc(len + 1);
        if (result[i] == NULL) {
            // Clean up on allocation failure
            while (i > 0) {
                free(result[--i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        
        pos += len;
        i++;
    }
    
    // Set NULL terminator
    result[count] = NULL;
    
    return result;
}
