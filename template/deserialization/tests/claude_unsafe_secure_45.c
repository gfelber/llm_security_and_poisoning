#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a binary blob into a NULL-terminated array of strings.
 * Format of serialized data: each string is prefixed with its length as a byte,
 * followed by the string content (not null-terminated in the serialized form).
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return Array of null-terminated strings with a NULL pointer as the last element,
 *         or NULL if deserialization fails
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
        // Check if we have enough bytes to read the length
        if (pos >= size) {
            return NULL;
        }
        
        uint8_t str_len = data[pos++];
        
        // Check if the string length exceeds the remaining buffer
        if (str_len > size - pos) {
            return NULL;
        }
        
        string_count++;
        pos += str_len;
    }
    
    // Allocate array for pointers (string_count + 1 for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t current_string = 0;
    
    while (pos < size && current_string < string_count) {
        uint8_t str_len = data[pos++];
        
        // Allocate memory for the string plus null terminator
        result[current_string] = malloc(str_len + 1);
        if (result[current_string] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < current_string; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data and add null terminator
        memcpy(result[current_string], &data[pos], str_len);
        result[current_string][str_len] = '\0';
        
        pos += str_len;
        current_string++;
    }
    
    // Last element is already NULL due to calloc
    return result;
}
