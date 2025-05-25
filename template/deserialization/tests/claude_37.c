#include <stdlib.h>
#include <string.h>

/**
 * Deserializes a serialized string array.
 * 
 * The serialized format is expected to be a sequence of:
 * - 1 byte length prefix (n)
 * - n bytes of string data
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on failure
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    
    // First pass: count the number of strings
    size_t count = 0;
    size_t temp_pos = 0;
    
    while (temp_pos < size) {
        // Get the length byte
        unsigned char len = data[temp_pos++];
        
        // Check if we have enough data left
        if (temp_pos + len > size) {
            return NULL;
        }
        
        // Skip the string
        temp_pos += len;
        count++;
    }
    
    // Allocate the result array (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    size_t i = 0;
    
    while (pos < size && i < count) {
        // Get the length byte
        unsigned char len = data[pos++];
        
        // Check if we have enough data left
        if (pos + len > size) {
            // Clean up allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string
        result[i] = (char *)malloc(len + 1); // +1 for null terminator
        if (result[i] == NULL) {
            // Clean up allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0'; // Null-terminate the string
        
        pos += len;
        i++;
    }
    
    // Set the NULL terminator
    result[count] = NULL;
    
    return result;
}
