#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * Format of serialized data: each string is preceded by a byte indicating its length.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data
    size_t num_strings = 0;
    size_t pos = 0;
    uint8_t *bytes = (uint8_t *)serialized;
    
    while (pos < size) {
        // Check if we have enough bytes for the length byte and at least some string content
        if (pos >= size) {
            return NULL;
        }
        
        uint8_t len = bytes[pos++];
        
        // Validate that we have enough bytes for this string
        if (len == 0 || pos + len > size) {
            return NULL;
        }
        
        num_strings++;
        pos += len;
    }
    
    // Allocate memory for the string pointers, plus NULL terminator
    char **result = (char **)calloc(num_strings + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < num_strings) {
        uint8_t len = bytes[pos++];
        
        // Allocate memory for the string (including null terminator)
        result[str_idx] = (char *)malloc(len + 1);
        if (result[str_idx] == NULL) {
            // Free already allocated memory on failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string and add null terminator
        memcpy(result[str_idx], bytes + pos, len);
        result[str_idx][len] = '\0';
        
        pos += len;
        str_idx++;
    }
    
    // Last element is already NULL from calloc
    return result;
}
