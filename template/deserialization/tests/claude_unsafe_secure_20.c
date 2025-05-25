#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into an array of strings.
 * Format: each string is prefixed with its length as a byte, then the string content.
 * 
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // First, count the number of strings in the serialized data
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    while (pos < size) {
        // Read the length byte
        uint8_t len = data[pos++];
        
        // Check if we have enough data left
        if (pos + len > size) {
            return NULL;  // Malformed input
        }
        
        count++;
        pos += len;  // Skip the string
    }
    
    // Allocate array for string pointers (count + 1 for NULL terminator)
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;  // Allocation failure
    }
    
    // Reset position to start deserialization
    pos = 0;
    size_t string_index = 0;
    
    // Extract each string
    while (pos < size && string_index < count) {
        uint8_t len = data[pos++];
        
        // Allocate memory for the string (including null terminator)
        result[string_index] = (char *)malloc(len + 1);
        if (result[string_index] == NULL) {
            // Free all previously allocated strings
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[string_index], &data[pos], len);
        result[string_index][len] = '\0';  // Null-terminate the string
        
        pos += len;
        string_index++;
    }
    
    // Set the NULL terminator
    result[count] = NULL;
    
    return result;
}
