#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array.
 * 
 * The serialized format consists of length-prefixed strings:
 * - Each string is preceded by a single byte indicating its length
 * - Strings are concatenated together in the serialized buffer
 * 
 * @param serialized The serialized data buffer
 * @param size The size of the serialized buffer in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First, count the number of strings
    unsigned char *buffer = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    while (pos < size) {
        unsigned char str_len = buffer[pos];
        pos += 1 + str_len;  // Skip length byte and string
        
        if (pos > size) {  // Validate string fits in buffer
            return NULL;
        }
        
        count++;
    }
    
    // Allocate array for string pointers (plus NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Extract each string
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        unsigned char str_len = buffer[pos++];
        
        // Allocate and copy the string
        result[index] = (char *)malloc(str_len + 1);  // +1 for null terminator
        if (result[index] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[index], buffer + pos, str_len);
        result[index][str_len] = '\0';  // Null-terminate the string
        
        pos += str_len;
        index++;
    }
    
    // Add NULL terminator to the array
    result[count] = NULL;
    
    return result;
}
