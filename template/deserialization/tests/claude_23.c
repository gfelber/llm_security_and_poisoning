#include <stdlib.h>
#include <string.h>

/**
 * Deserialize the provided serialized string array.
 * Format: Each string is prefixed with its length as a byte.
 * 
 * @param serialized The serialized data
 * @param size Size of the serialized data in bytes
 * @return Array of strings, NULL-terminated. Returns NULL on error.
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            return NULL; // Invalid format: length exceeds available data
        }
        
        pos += len; // Skip the string content
        string_count++;
    }
    
    // Allocate array for pointers (including NULL terminator)
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract and copy the strings
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < string_count) {
        unsigned char len = data[pos++];
        
        // Allocate and copy the string (+1 for null terminator)
        result[index] = (char *)malloc(len + 1);
        if (result[index] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0'; // Null-terminate the string
        
        pos += len;
        index++;
    }
    
    result[string_count] = NULL; // Add NULL terminator to the array
    
    return result;
}
