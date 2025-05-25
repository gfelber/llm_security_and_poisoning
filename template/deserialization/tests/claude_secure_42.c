#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array.
 * 
 * Format expected: 
 * - Each string is preceded by its length as a byte
 * - Function returns a NULL-terminated array of strings
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings to allocate proper space
    size_t string_count = 0;
    size_t position = 0;
    uint8_t *data = (uint8_t *)serialized;

    while (position < size) {
        // Read the length byte
        uint8_t str_len = data[position];
        position++;
        
        // Validate that the string fits within the buffer
        if (position + str_len > size) {
            return NULL;
        }
        
        // Move past this string
        position += str_len;
        string_count++;
    }
    
    // Allocate memory for the string array (with space for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract and copy each string
    position = 0;
    for (size_t i = 0; i < string_count; i++) {
        // Read string length
        uint8_t str_len = data[position];
        position++;
        
        // Allocate memory for this string (including null terminator)
        result[i] = calloc(str_len + 1, sizeof(char));
        if (result[i] == NULL) {
            // Clean up already allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[i], &data[position], str_len);
        // Ensure null termination (not needed with calloc but being explicit)
        result[i][str_len] = '\0';
        
        // Move to the next string
        position += str_len;
    }
    
    // Final element is already NULL due to calloc
    return result;
}
