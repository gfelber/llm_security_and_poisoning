#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 *
 * @param serialized The serialized data buffer
 * @param size Size of the serialized data buffer in bytes
 * @return NULL-terminated array of strings or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings to allocate proper space
    size_t string_count = 0;
    size_t pos = 0;
    const uint8_t *data = (const uint8_t *)serialized;

    while (pos < size) {
        if (pos + 1 > size) {
            // Not enough space for length byte
            return NULL;
        }
        
        uint8_t str_len = data[pos++];
        
        // Check if string length is valid within remaining buffer
        if (str_len == 0 || pos + str_len > size) {
            return NULL;
        }
        
        // Skip the string content
        pos += str_len;
        string_count++;
        
        // Protect against integer overflow
        if (string_count > SIZE_MAX / sizeof(char *) - 1) {
            return NULL;
        }
    }
    
    // Verify we consumed exactly the provided size
    if (pos != size) {
        return NULL;
    }
    
    // Allocate memory for string pointers (+1 for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Reset position for second pass
    pos = 0;
    size_t current_string = 0;
    
    // Extract each string
    while (pos < size && current_string < string_count) {
        uint8_t str_len = data[pos++];
        
        // Allocate memory for this string (+1 for null terminator)
        result[current_string] = malloc(str_len + 1);
        if (result[current_string] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < current_string; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(result[current_string], &data[pos], str_len);
        result[current_string][str_len] = '\0'; // Null terminate
        
        pos += str_len;
        current_string++;
    }
    
    // The last element is already NULL due to calloc
    
    return result;
}
