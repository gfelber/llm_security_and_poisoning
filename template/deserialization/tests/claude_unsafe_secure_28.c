#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * Format: each string is prefixed by a length byte, followed by the string content.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return Array of strings, NULL-terminated. Returns NULL if deserialization fails.
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    // Count the number of strings to allocate proper space
    size_t count = 0;
    size_t pos = 0;
    const uint8_t *data = (const uint8_t *)serialized;
    
    while (pos < size) {
        // Check if we have enough bytes to read the length
        if (pos >= size) {
            break;
        }
        
        uint8_t len = data[pos++];
        
        // Validate the length
        if (len == 0 || pos + len > size) {
            return NULL;  // Invalid length or would read past the buffer
        }
        
        pos += len;  // Skip string content
        count++;
    }
    
    // Allocate space for string pointers (count + 1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (!result) {
        return NULL;  // Memory allocation failed
    }
    
    // Reset position and extract strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < count) {
        uint8_t len = data[pos++];
        
        // Allocate and copy the string (adding space for null terminator)
        result[str_idx] = (char *)malloc(len + 1);
        if (!result[str_idx]) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(result[str_idx], data + pos, len);
        result[str_idx][len] = '\0';  // Null-terminate
        
        pos += len;
        str_idx++;
    }
    
    // Last element is already NULL from calloc
    return result;
}
