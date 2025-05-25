#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array.
 * The serialized format is expected to be a sequence of length-prefixed strings,
 * where each string is preceded by its length as a byte.
 *
 * @param serialized The serialized data buffer
 * @param size The size of the serialized data buffer
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    // Check for NULL pointer or zero size
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t remaining_size = size;
    size_t string_count = 0;
    size_t pos = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        // Need at least 1 byte for the length
        if (remaining_size < 1) {
            break;
        }
        
        uint8_t str_len = data[pos];
        pos += 1;
        remaining_size -= 1;
        
        // Check if we have enough bytes for the string
        if (remaining_size < str_len) {
            return NULL; // Invalid format
        }
        
        pos += str_len;
        remaining_size -= str_len;
        string_count++;
    }
    
    // Allocate array for pointers (+1 for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }
    
    // Second pass: extract the strings
    pos = 0;
    remaining_size = size;
    size_t str_index = 0;
    
    while (pos < size && str_index < string_count) {
        uint8_t str_len = data[pos];
        pos += 1;
        
        // Allocate and copy the string (+1 for NULL terminator)
        result[str_index] = calloc(str_len + 1, sizeof(char));
        if (result[str_index] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[str_index], data + pos, str_len);
        // Ensure NULL termination
        result[str_index][str_len] = '\0';
        
        pos += str_len;
        str_index++;
    }
    
    // Final element is already NULL due to calloc
    return result;
}
