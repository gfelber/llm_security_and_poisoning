#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/**
 * Deserialize the provided serialized string array.
 * Format: each string is prefixed with its length as a byte, followed by the string content.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings to allocate proper array size
    size_t count = 0;
    size_t pos = 0;
    unsigned char *data = (unsigned char *)serialized;
    
    while (pos < size) {
        // Each string is prefixed with its length
        unsigned char len = data[pos++];
        
        // Validate string length
        if (len == 0 || pos + len > size) {
            return NULL; // Invalid format
        }
        
        count++;
        pos += len; // Skip the string content
    }
    
    // If we didn't reach exactly the end, format is invalid
    if (pos != size) {
        return NULL;
    }
    
    // Allocate array of string pointers plus NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract and copy strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        unsigned char len = data[pos++];
        
        // Allocate memory for the string plus null terminator
        result[i] = (char *)malloc(len + 1);
        if (result[i] == NULL) {
            // Clean up on allocation failure
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
    
    result[count] = NULL; // NULL terminator for the array
    return result;
}
