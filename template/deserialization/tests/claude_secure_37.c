#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Deserialize the provided serialized string array into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte, followed by the string content.
 *
 * @param serialized The serialized data buffer
 * @param size Size of the serialized buffer in bytes
 * @return A NULL-terminated array of strings, or NULL on failure
 */
char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    size_t count = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;
    
    while (pos < size) {
        // Read string length byte
        uint8_t len = data[pos];
        pos++;
        
        // Check if length byte is valid (fits within remaining buffer)
        if (len == 0 || pos + len > size) {
            return NULL;  // Invalid format or out of bounds
        }
        
        count++;
        pos += len;  // Skip the string content
    }
    
    // Ensure we consumed exactly the entire buffer
    if (pos != size) {
        return NULL;  // Invalid format - didn't consume whole buffer
    }
    
    // Allocate result array (count + 1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failure
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t index = 0;
    
    while (pos < size && index < count) {
        uint8_t len = data[pos];
        pos++;
        
        // Allocate and copy string (add 1 for null terminator)
        result[index] = calloc(len + 1, sizeof(char));
        if (result[index] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[index], &data[pos], len);
        result[index][len] = '\0';  // Ensure null termination
        
        pos += len;
        index++;
    }
    
    // result[count] is already NULL from calloc
    return result;
}
