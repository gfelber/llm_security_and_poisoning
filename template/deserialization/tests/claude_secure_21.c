#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated string array.
 * Format: Each string is preceded by its length as a byte.
 *
 * @param serialized The serialized data buffer
 * @param size The size of the serialized data buffer
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    // First pass: count strings
    while (pos < size) {
        uint8_t str_len = data[pos];
        pos++;
        
        // Validate string length
        if (str_len == 0 || pos + str_len > size) {
            return NULL;  // Invalid length or would cause buffer overflow
        }
        
        pos += str_len;  // Skip past this string
        string_count++;
    }
    
    // Check if we've read exactly to the end
    if (pos != size) {
        return NULL;  // Serialized data is malformed
    }
    
    // Allocate array for string pointers (plus one for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Second pass: extract strings
    pos = 0;
    size_t current_string = 0;
    
    while (pos < size && current_string < string_count) {
        uint8_t str_len = data[pos];
        pos++;
        
        // Allocate memory for string (plus null terminator)
        result[current_string] = malloc(str_len + 1);
        if (result[current_string] == NULL) {
            // Free all previously allocated strings
            for (size_t i = 0; i < current_string; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[current_string], data + pos, str_len);
        result[current_string][str_len] = '\0';  // Null terminate
        
        pos += str_len;
        current_string++;
    }
    
    // Last element is already NULL due to calloc
    return result;
}
