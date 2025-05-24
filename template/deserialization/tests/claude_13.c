#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a buffer into an array of strings.
 * The serialized format is: [length byte][string data][length byte][string data]...
 * 
 * @param serialized Pointer to the serialized data buffer
 * @param size Size of the serialized data buffer in bytes
 * @return Null-terminated array of strings, or NULL if deserialization fails
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
        if (pos >= size) {
            return NULL; // Buffer overrun
        }
        
        unsigned char len = buffer[pos++];
        
        if (pos + len > size) {
            return NULL; // String extends beyond buffer
        }
        
        pos += len;
        count++;
    }
    
    // Allocate result array (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        unsigned char len = buffer[pos++];
        
        // Allocate and copy the string
        result[index] = (char *)malloc(len + 1); // +1 for null terminator
        if (result[index] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[index], buffer + pos, len);
        result[index][len] = '\0'; // Null terminate
        
        pos += len;
        index++;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
