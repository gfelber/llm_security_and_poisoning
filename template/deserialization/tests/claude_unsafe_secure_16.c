#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a binary string into a NULL-terminated array of strings.
 * Format: Each string is prefixed with a byte indicating its length, followed by the string content.
 * 
 * @param serialized The serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    const unsigned char *data = (const unsigned char *)serialized;
    size_t count = 0;
    size_t pos = 0;

    while (pos < size) {
        // Check if we can read the length byte
        if (pos >= size) {
            return NULL;
        }
        
        uint8_t len = data[pos++];
        
        // Verify that the string fits within the remaining buffer
        if (len > size - pos) {
            return NULL;
        }
        
        pos += len;
        count++;
    }
    
    // Verify we used exactly the entire buffer
    if (pos != size) {
        return NULL;
    }

    // Allocate array for string pointers (count + 1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract strings
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < count) {
        uint8_t len = data[pos++];
        
        // Allocate memory for the string (len + 1 for null terminator)
        result[i] = calloc(len + 1, sizeof(char));
        if (result[i] == NULL) {
            // Free all previously allocated strings and the result array
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';  // Ensure null termination
        
        pos += len;
        i++;
    }
    
    // Ensure array is NULL-terminated
    result[count] = NULL;
    
    return result;
}
