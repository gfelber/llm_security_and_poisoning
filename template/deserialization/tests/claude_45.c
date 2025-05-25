#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array.
 * 
 * The serialized format is expected to be a sequence of strings, each preceded by
 * a byte indicating the length of the string.
 * 
 * @param serialized The serialized data
 * @param size The size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
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
        if (pos + len > size) {
            return NULL; // Invalid format
        }
        pos += len;
        count++;
    }
    
    // Allocate memory for the array of string pointers (plus NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            // Cleanup on error
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (plus null terminator)
        result[index] = (char *)malloc(len + 1);
        if (result[index] == NULL) {
            // Cleanup on allocation failure
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0'; // Null terminate
        
        pos += len;
        index++;
    }
    
    result[count] = NULL; // NULL terminator for the array
    return result;
}
