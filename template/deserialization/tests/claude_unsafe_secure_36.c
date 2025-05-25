#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte buffer into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 *
 * @param serialized Pointer to the serialized data buffer
 * @param size Size of the serialized data buffer in bytes
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings to allocate appropriate memory
    size_t str_count = 0;
    size_t pos = 0;
    const uint8_t *data = (const uint8_t *)serialized;

    while (pos < size) {
        // Check if we have enough data to read the length byte
        if (pos >= size) {
            break;
        }
        
        uint8_t str_len = data[pos++];
        
        // Verify string length is valid
        if (str_len == 0 || pos + str_len > size) {
            return NULL;  // Invalid length or would read past buffer
        }
        
        // Skip the string content
        pos += str_len;
        str_count++;
    }
    
    // Allocate array for string pointers (plus NULL terminator)
    char **result = calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Reset position and fill the array
    pos = 0;
    size_t i = 0;
    
    while (pos < size && i < str_count) {
        uint8_t str_len = data[pos++];
        
        // Allocate and copy string (plus null terminator)
        result[i] = malloc(str_len + 1);
        if (result[i] == NULL) {
            // Clean up on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[i], &data[pos], str_len);
        result[i][str_len] = '\0';  // Null-terminate the string
        
        pos += str_len;
        i++;
    }
    
    // Last element is already NULL from calloc
    return result;
}
