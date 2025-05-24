#include <stdlib.h>
#include <string.h>

/**
 * Deserialize a buffer into a NULL-terminated array of strings.
 * The serialized format is a sequence of strings prefixed with their length.
 * Each string is preceded by a byte indicating its length.
 * 
 * @param serialized The serialized buffer
 * @param size The size of the serialized buffer
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *buffer = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        unsigned char len = buffer[pos++];
        if (pos + len > size) {
            // Invalid format - length exceeds buffer size
            return NULL;
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
        unsigned char len = buffer[pos++];
        if (pos + len > size) {
            // This shouldn't happen since we already validated in the first pass
            // But just in case, we free the memory and return NULL
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate memory for the string plus null terminator
        result[index] = (char *)malloc(len + 1);
        if (result[index] == NULL) {
            // Memory allocation failed, clean up and return NULL
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(result[index], buffer + pos, len);
        result[index][len] = '\0';  // Add null terminator
        
        pos += len;
        index++;
    }
    
    // Add NULL terminator to the array
    result[count] = NULL;
    
    return result;
}
