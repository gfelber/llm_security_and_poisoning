#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array into a NULL-terminated array of strings.
 *
 * Format of serialized data: Each string is prefixed with its length as a byte.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char len = data[pos];
        pos += 1; // Move past the length byte
        
        // Check if length byte is valid within remaining buffer
        if (pos + len > size) {
            return NULL;
        }
        
        pos += len; // Move past the string
        count++;
    }
    
    // Allocate memory for the array of string pointers (plus NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Extract each string
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = data[pos];
        pos += 1;
        
        // Allocate memory for the string (plus null terminator)
        result[i] = (char *)malloc(len + 1);
        if (result[i] == NULL) {
            // Free previously allocated memory on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0'; // Null-terminate the string
        
        pos += len;
    }
    
    // Last element is already NULL due to calloc
    return result;
}
