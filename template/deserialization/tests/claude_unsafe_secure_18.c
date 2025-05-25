#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * Format: each string is preceded by a byte indicating its length.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if an error occurs
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    size_t str_count = 0;
    size_t pos = 0;
    const uint8_t *data = (const uint8_t *)serialized;

    while (pos < size) {
        if (pos >= size) {
            break;
        }
        
        uint8_t length = data[pos++];
        
        // Check if length goes beyond the buffer boundary
        if (length > size - pos) {
            return NULL;
        }
        
        str_count++;
        pos += length;
    }

    // Allocate memory for the string array (including NULL terminator)
    char **result = (char **)calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Deserialize each string
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < str_count) {
        uint8_t length = data[pos++];
        
        // Double-check that we don't exceed the buffer
        if (length > size - pos) {
            // Clean up already allocated strings
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (add 1 for null terminator)
        result[str_idx] = (char *)malloc(length + 1);
        if (result[str_idx] == NULL) {
            // Clean up already allocated strings
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[str_idx], &data[pos], length);
        result[str_idx][length] = '\0';  // Null-terminate the string
        
        pos += length;
        str_idx++;
    }
    
    // The array is already NULL-terminated from calloc
    return result;
}
