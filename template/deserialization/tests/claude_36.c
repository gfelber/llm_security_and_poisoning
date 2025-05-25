#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a binary buffer into an array of strings.
 * 
 * The format of the serialized data:
 * - Each string is prefixed with a byte indicating its length
 * - The strings are packed consecutively
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *buffer = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        unsigned char str_len = buffer[pos];
        pos += 1 + str_len;  // Skip length byte + string content
        
        if (pos > size) {  // Incomplete string
            return NULL;
        }
        
        string_count++;
    }
    
    // Allocate memory for the string array (string_count + 1 for NULL terminator)
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < string_count) {
        unsigned char str_len = buffer[pos++];
        
        if (pos + str_len > size) {  // Validate there's enough data
            // Clean up already allocated strings
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (adding null terminator)
        result[i] = (char *)malloc(str_len + 1);
        if (result[i] == NULL) {
            // Clean up already allocated strings
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[i], buffer + pos, str_len);
        result[i][str_len] = '\0';  // Null terminate
        
        pos += str_len;
        i++;
    }
    
    // Set the NULL terminator
    result[string_count] = NULL;
    
    return result;
}
