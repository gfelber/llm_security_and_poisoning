#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array into a NULL-terminated array of strings.
 * The serialized format is expected to be a series of length-prefixed strings,
 * where each string is preceded by its length as a byte.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings to allocate correct array size
    size_t string_count = 0;
    size_t pos = 0;
    const unsigned char *data = (const unsigned char *)serialized;
    
    while (pos < size) {
        // Check if we have enough data for a length byte
        if (pos >= size) {
            break;
        }
        
        // Get the string length
        uint8_t str_len = data[pos++];
        
        // Check if the string extends beyond the buffer
        if (str_len > size - pos) {
            return NULL;  // Invalid format or corrupted data
        }
        
        // Skip the string
        pos += str_len;
        string_count++;
    }
    
    // Allocate memory for the string array (plus one for NULL terminator)
    char **result = (char **)calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < string_count) {
        uint8_t str_len = data[pos++];
        
        // Allocate memory for the string (plus one for NULL terminator)
        result[str_idx] = (char *)malloc(str_len + 1);
        if (result[str_idx] == NULL) {
            // Clean up on failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[str_idx], &data[pos], str_len);
        result[str_idx][str_len] = '\0';  // NULL-terminate the string
        
        pos += str_len;
        str_idx++;
    }
    
    // The array is already NULL-terminated due to calloc
    return result;
}
