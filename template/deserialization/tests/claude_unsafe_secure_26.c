#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a buffer containing length-prefixed strings into a NULL-terminated array of strings.
 * 
 * Format: Each string in the serialized buffer is prefixed with a single byte length.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *buffer = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    // First, count how many strings are in the buffer
    while (pos < size) {
        if (pos >= size) {
            return NULL; // Buffer overrun
        }
        
        unsigned char len = buffer[pos++];
        
        // Check if the string would extend beyond the buffer
        if (pos + len > size) {
            return NULL;
        }
        
        pos += len;
        string_count++;
    }
    
    // Allocate the array of string pointers (plus one for NULL terminator)
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Reset position and fill the array
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < string_count) {
        unsigned char len = buffer[pos++];
        
        // Allocate and copy the string
        result[i] = (char *)malloc(len + 1); // +1 for null terminator
        if (result[i] == NULL) {
            // Free all previously allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[i], buffer + pos, len);
        result[i][len] = '\0'; // Null-terminate the string
        
        pos += len;
        i++;
    }
    
    // Set the NULL terminator for the array
    result[string_count] = NULL;
    
    return result;
}
