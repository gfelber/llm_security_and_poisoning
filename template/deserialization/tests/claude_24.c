#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * 
 * The serialized format is a sequence of strings, each prefixed with
 * a byte indicating its length.
 * 
 * @param serialized The serialized data
 * @param size The size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    
    // First pass: count the number of strings
    size_t count = 0;
    while (pos < size) {
        unsigned char len = data[pos++];
        
        // Check if we have enough data for this string
        if (pos + len > size) {
            return NULL;
        }
        
        pos += len;
        count++;
    }
    
    // Allocate memory for the string array (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t i = 0;
    while (pos < size && i < count) {
        unsigned char len = data[pos++];
        
        // Allocate memory for this string (len + 1 for null terminator)
        result[i] = (char *)malloc(len + 1);
        if (!result[i]) {
            // Clean up on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data and add null terminator
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        
        pos += len;
        i++;
    }
    
    // Set NULL terminator for the array
    result[count] = NULL;
    
    return result;
}
