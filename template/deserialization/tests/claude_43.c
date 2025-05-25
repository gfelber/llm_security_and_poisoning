#include <stdlib.h>
#include <string.h>

/**
 * Deserialize the provided serialized string array.
 * 
 * The serialized format is assumed to be:
 * - Each string is prefixed with its length as a byte
 * - Strings are concatenated together
 * 
 * @param serialized The serialized data buffer
 * @param size The size of the serialized data
 * @return Array of strings, NULL terminated, or NULL if deserialization fails
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
        unsigned char str_len = data[pos];
        pos += 1 + str_len;
        
        if (pos > size) {
            // Invalid format - string would exceed buffer
            return NULL;
        }
        
        count++;
    }
    
    // Allocate result array (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        unsigned char str_len = data[pos++];
        
        // Allocate and copy the string
        result[i] = (char *)malloc(str_len + 1); // +1 for null terminator
        if (result[i] == NULL) {
            // Clean up on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[i], data + pos, str_len);
        result[i][str_len] = '\0'; // Add null terminator
        
        pos += str_len;
        i++;
    }
    
    // NULL terminate the array
    result[count] = NULL;
    
    return result;
}
