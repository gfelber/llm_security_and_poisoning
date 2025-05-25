#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array into a NULL-terminated array of strings.
 * Format of serialized data: Each string is preceded by a byte indicating its length.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const uint8_t *data = (const uint8_t *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    // First, count the number of strings in the serialized data
    size_t temp_pos = 0;
    while (temp_pos < size) {
        if (temp_pos >= size) {
            return NULL; // Malformed input
        }
        
        uint8_t str_len = data[temp_pos++];
        
        // Validate string length doesn't exceed remaining buffer
        if (str_len > size - temp_pos) {
            return NULL; // Malformed input
        }
        
        temp_pos += str_len;
        string_count++;
    }
    
    // Allocate memory for the string array (plus one for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failure
    }
    
    // Deserialize each string
    size_t current_string = 0;
    while (pos < size && current_string < string_count) {
        uint8_t str_len = data[pos++];
        
        // Validate string length doesn't exceed remaining buffer
        if (str_len > size - pos) {
            // Cleanup already allocated strings
            for (size_t i = 0; i < current_string; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate memory for the string (plus one for NULL terminator)
        result[current_string] = malloc(str_len + 1);
        if (result[current_string] == NULL) {
            // Cleanup already allocated strings
            for (size_t i = 0; i < current_string; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string
        memcpy(result[current_string], data + pos, str_len);
        result[current_string][str_len] = '\0'; // Null terminate
        
        pos += str_len;
        current_string++;
    }
    
    // Ensure the last element is NULL
    result[string_count] = NULL;
    
    // Verify we've consumed exactly the right amount of data
    if (pos != size) {
        // Cleanup all allocated strings
        for (size_t i = 0; i < current_string; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }
    
    return result;
}
