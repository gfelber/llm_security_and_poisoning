#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * The serialized format consists of length-prefixed strings where each string
 * is preceded by a byte that indicates its length.
 *
 * @param serialized The serialized data to parse
 * @param size The size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    size_t str_count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;

    while (pos < size) {
        // Check if we have enough bytes to read the length
        if (pos >= size) {
            break;
        }
        
        uint8_t len = data[pos++];
        
        // Ensure we don't read beyond the buffer
        if (len > size - pos) {
            return NULL;
        }
        
        str_count++;
        pos += len;
    }

    // Allocate array for string pointers (plus NULL terminator)
    char **result = calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Parse each string
    pos = 0;
    size_t idx = 0;
    
    while (pos < size && idx < str_count) {
        uint8_t len = data[pos++];
        
        // Ensure we don't read beyond the buffer
        if (len > size - pos) {
            // Clean up already allocated memory
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (adding null terminator)
        result[idx] = malloc(len + 1);
        if (result[idx] == NULL) {
            // Clean up already allocated memory
            for (size_t i = 0; i < idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[idx], &data[pos], len);
        result[idx][len] = '\0';
        
        pos += len;
        idx++;
    }
    
    // Ensure the array is NULL-terminated
    result[idx] = NULL;
    
    return result;
}
