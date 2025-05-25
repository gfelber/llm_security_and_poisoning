#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserialize a byte array into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on failure
 */
char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings to allocate proper space
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    while (pos < size) {
        // Each string starts with its length byte
        uint8_t len = data[pos];
        pos++;
        
        // Bounds check for string length
        if (len == 0 || pos + len > size) {
            return NULL;  // Invalid format or would read past buffer
        }
        
        pos += len;  // Skip string content
        count++;     // Count this string
    }
    
    // If we didn't end exactly at the buffer boundary, format is invalid
    if (pos != size) {
        return NULL;
    }
    
    // Allocate array for string pointers (count + 1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failure
    }
    
    // Reset position and fill the array
    pos = 0;
    size_t string_index = 0;
    
    while (pos < size && string_index < count) {
        uint8_t len = data[pos];
        pos++;
        
        // Allocate memory for the string (including null terminator)
        result[string_index] = (char *)malloc(len + 1);
        if (result[string_index] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string contents
        memcpy(result[string_index], data + pos, len);
        result[string_index][len] = '\0';  // Ensure null termination
        
        pos += len;
        string_index++;
    }
    
    // Last element is already NULL because of calloc
    return result;
}
