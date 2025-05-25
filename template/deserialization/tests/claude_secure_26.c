#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * 
 * Format: Each string is prefixed with its length as a byte.
 * 
 * @param serialized The serialized buffer
 * @param size Size of the serialized buffer in bytes
 * @return Dynamically allocated NULL-terminated array of strings or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *buffer = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        // Check if we have enough bytes to read the length
        if (pos >= size) {
            return NULL;  // Unexpected end of buffer
        }
        
        uint8_t len = buffer[pos++];
        
        // Validate string length
        if (len == 0 || pos + len > size) {
            return NULL;  // Invalid length or buffer overflow
        }
        
        pos += len;  // Skip string content
        count++;     // Increment string counter
    }
    
    // Allocate memory for string pointers (count + 1 for NULL terminator)
    // Check for overflow before allocation
    if (count > SIZE_MAX / sizeof(char *) - 1) {
        return NULL;  // Would overflow
    }
    
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        uint8_t len = buffer[pos++];
        
        // Allocate memory for the string (len + 1 for NULL terminator)
        char *str = (char *)malloc(len + 1);
        if (str == NULL) {
            // Clean up allocated memory on failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(str, buffer + pos, len);
        str[len] = '\0';  // NULL terminate
        
        result[str_idx++] = str;
        pos += len;
    }
    
    // Ensure NULL termination of the array (already done by calloc, but being explicit)
    result[count] = NULL;
    
    return result;
}
