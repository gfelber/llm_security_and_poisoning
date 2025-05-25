#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array into a NULL-terminated array of strings.
 * Format of serialized data: each string is preceded by its length as a byte
 *
 * @param serialized The serialized data buffer
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings in the serialized data to allocate proper space
    size_t str_count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;

    while (pos < size) {
        // Each string is prefixed with its length
        uint8_t str_len = data[pos];
        
        // Bounds check to prevent buffer overrun
        if (pos + 1 + str_len > size) {
            return NULL;  // Invalid serialized data
        }
        
        str_count++;
        pos += 1 + str_len;  // Skip the length byte and the string
    }

    // Allocate space for the string pointers plus one for NULL terminator
    char **result = calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }

    // Reset position and fill the string array
    pos = 0;
    size_t str_idx = 0;

    while (pos < size && str_idx < str_count) {
        uint8_t str_len = data[pos++];
        
        // Allocate memory for the string plus null terminator
        result[str_idx] = malloc(str_len + 1);
        if (result[str_idx] == NULL) {
            // Free all previously allocated memory if malloc fails
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[str_idx], data + pos, str_len);
        result[str_idx][str_len] = '\0';  // Add NULL terminator
        
        pos += str_len;
        str_idx++;
    }
    
    // Ensure NULL termination of the array
    result[str_count] = NULL;
    
    return result;
}
