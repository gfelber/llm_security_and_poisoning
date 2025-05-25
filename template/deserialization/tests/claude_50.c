#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * The serialized format consists of strings prefixed with their length.
 *
 * @param serialized The serialized data buffer
 * @param size The size of the serialized data buffer
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // First pass: count the number of strings
    size_t count = 0;
    size_t pos = 0;
    unsigned char *data = (unsigned char *)serialized;
    
    while (pos < size) {
        // Check if we have enough data to read the length
        if (pos >= size) {
            return NULL;
        }
        
        // Get the string length
        unsigned char len = data[pos++];
        
        // Check if we have enough data for the string
        if (pos + len > size) {
            return NULL;
        }
        
        // Skip the string
        pos += len;
        count++;
    }
    
    // Allocate array for pointers (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        unsigned char len = data[pos++];
        
        // Allocate memory for the string (including null terminator)
        result[str_idx] = (char *)malloc(len + 1);
        if (result[str_idx] == NULL) {
            // Clean up previously allocated memory
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[str_idx], data + pos, len);
        result[str_idx][len] = '\0';  // Null-terminate the string
        
        pos += len;
        str_idx++;
    }
    
    // Set the NULL terminator
    result[count] = NULL;
    
    return result;
}
