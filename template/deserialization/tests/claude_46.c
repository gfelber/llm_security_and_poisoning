#include <stdlib.h>
#include <string.h>

/**
 * Deserialize a serialized string array.
 * 
 * The serialized format is a sequence of strings where each string is
 * preceded by its length as a byte.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // Count number of strings in the serialized data
    unsigned char *data = (unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        unsigned char len = data[pos];
        pos += 1;
        
        // Validate that we have enough data for this string
        if (pos + len > size) {
            return NULL;
        }
        
        pos += len;
        count++;
    }
    
    // Allocate memory for the string array (plus NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize all pointers to NULL
    memset(result, 0, (count + 1) * sizeof(char *));
    
    // Deserialize each string
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        unsigned char len = data[pos];
        pos += 1;
        
        // Allocate memory for the string (plus null terminator)
        result[index] = (char *)malloc(len + 1);
        if (result[index] == NULL) {
            // Free all previously allocated memory
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';  // Null-terminate the string
        
        pos += len;
        index++;
    }
    
    // Ensure the array is NULL-terminated
    result[index] = NULL;
    
    return result;
}
