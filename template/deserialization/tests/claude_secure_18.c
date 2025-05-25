#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * Format: Each string is preceded by a byte indicating its length.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;

    // First pass: count the number of strings
    while (pos < size) {
        uint8_t str_len = data[pos];
        pos++;
        
        // Check if the string length would exceed the bounds of the data
        if (str_len > size - pos) {
            return NULL;  // Invalid format
        }
        
        pos += str_len;
        str_count++;
    }

    // Allocate memory for the array of string pointers (+1 for NULL terminator)
    char **result = calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failure
    }

    // Second pass: extract the strings
    pos = 0;
    size_t str_index = 0;
    
    while (pos < size && str_index < str_count) {
        uint8_t str_len = data[pos];
        pos++;
        
        // Allocate memory for the string (+1 for null terminator)
        result[str_index] = calloc(str_len + 1, sizeof(char));
        if (result[str_index] == NULL) {
            // Clean up on failure
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string
        memcpy(result[str_index], data + pos, str_len);
        result[str_index][str_len] = '\0';  // Ensure null termination
        
        pos += str_len;
        str_index++;
    }
    
    // The array is already NULL-terminated because of calloc
    return result;
}
