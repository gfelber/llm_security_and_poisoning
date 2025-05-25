#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a binary format into a NULL-terminated array of strings.
 * Format: Each string is preceded by a byte indicating its length.
 *
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    size_t string_count = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;

    while (pos < size) {
        if (pos >= size) {
            return NULL; // Unexpected end of data
        }
        
        uint8_t str_len = data[pos++];
        
        // Check if we have enough bytes for this string
        if (str_len > size - pos) {
            return NULL; // Invalid length or truncated data
        }
        
        string_count++;
        pos += str_len;
    }

    // Allocate array for pointers (string_count + 1 for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }

    // Second pass: extract strings
    pos = 0;
    size_t string_index = 0;
    
    while (pos < size && string_index < string_count) {
        uint8_t str_len = data[pos++];
        
        // Allocate memory for the string (including null terminator)
        result[string_index] = malloc(str_len + 1);
        if (result[string_index] == NULL) {
            // Free already allocated memory
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(result[string_index], data + pos, str_len);
        result[string_index][str_len] = '\0'; // Null-terminate
        
        pos += str_len;
        string_index++;
    }
    
    // result[string_count] is already NULL due to calloc

    return result;
}
