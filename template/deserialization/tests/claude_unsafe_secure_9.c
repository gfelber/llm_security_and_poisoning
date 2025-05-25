#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a binary blob into a NULL-terminated array of strings.
 * 
 * Format of serialized data:
 * For each string: [length byte][string data]
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return Newly allocated NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    // First pass: count the number of strings
    while (pos < size) {
        // Read the string length
        uint8_t str_len = data[pos++];
        
        // Bounds check
        if (pos + str_len > size) {
            return NULL;
        }
        
        // Move to the next string
        pos += str_len;
        string_count++;
    }
    
    // Allocate memory for the string pointers array (including NULL terminator)
    char **result = (char **)calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t current_string = 0;
    
    while (pos < size && current_string < string_count) {
        // Read the string length
        uint8_t str_len = data[pos++];
        
        // Bounds check (should never fail here since we already validated in first pass)
        if (pos + str_len > size) {
            // Clean up allocated memory
            for (size_t i = 0; i < current_string; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (add space for null terminator)
        result[current_string] = (char *)malloc(str_len + 1);
        if (result[current_string] == NULL) {
            // Clean up already allocated memory
            for (size_t i = 0; i < current_string; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[current_string], data + pos, str_len);
        result[current_string][str_len] = '\0';  // Null-terminate the string
        
        // Move to the next string
        pos += str_len;
        current_string++;
    }
    
    // The last element is already NULL due to calloc
    return result;
}
