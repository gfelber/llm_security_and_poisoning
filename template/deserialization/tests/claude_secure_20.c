#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * Format: each string is preceded by its length as a byte.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    // Safety checks
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t position = 0;
    size_t string_count = 0;
    
    // First pass: count the number of strings
    while (position < size) {
        // Each string is preceded by its length as a byte
        uint8_t str_len = data[position];
        position++;
        
        // Check for buffer overrun
        if (position + str_len > size) {
            return NULL;
        }
        
        position += str_len;
        string_count++;
    }
    
    // Check if the serialized data was malformed
    if (position != size) {
        return NULL;
    }
    
    // Allocate array of string pointers (+1 for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract strings
    position = 0;
    size_t current_string = 0;
    
    while (position < size && current_string < string_count) {
        uint8_t str_len = data[position];
        position++;
        
        // Allocate memory for the string (+1 for null terminator)
        result[current_string] = calloc(str_len + 1, sizeof(char));
        if (result[current_string] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < current_string; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string
        memcpy(result[current_string], data + position, str_len);
        // Ensure null termination
        result[current_string][str_len] = '\0';
        
        position += str_len;
        current_string++;
    }
    
    // Ensure the last element is NULL (calloc should have done this already)
    result[string_count] = NULL;
    
    return result;
}
