#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

/**
 * Deserializes a buffer into an array of strings.
 * 
 * Format: Each string is prefixed by a byte indicating its length, followed by
 * the string content (without null terminator in the serialized form).
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 *         Caller is responsible for freeing the returned array and all strings
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Cast the void pointer to unsigned char for safe byte-by-byte access
    const unsigned char *data = (const unsigned char *)serialized;
    
    // First pass: count the number of strings to allocate the array
    size_t string_count = 0;
    size_t pos = 0;
    
    while (pos < size) {
        // Check if we have at least one byte for the length
        if (pos >= size) {
            break;
        }
        
        // Get string length
        uint8_t str_len = data[pos++];
        
        // Ensure we don't go out of bounds
        if (str_len > size - pos) {
            return NULL;  // Invalid format or corrupted data
        }
        
        // Skip the string content
        pos += str_len;
        string_count++;
    }
    
    // Allocate array for pointers (+1 for NULL terminator)
    // Check for potential size_t overflow
    if (string_count > SIZE_MAX / sizeof(char *) - 1) {
        return NULL;  // Would overflow
    }
    
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Second pass: deserialize each string
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < string_count) {
        uint8_t str_len = data[pos++];
        
        // Check if string length is valid
        if (str_len > size - pos) {
            // Clean up already allocated memory
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate memory for the string (+1 for null terminator)
        // Check for integer overflow
        if (str_len >= SIZE_MAX) {
            // Clean up already allocated memory
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        char *str = calloc(str_len + 1, sizeof(char));
        if (str == NULL) {
            // Clean up already allocated memory
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string content
        memcpy(str, data + pos, str_len);
        str[str_len] = '\0';  // Ensure null termination
        
        result[str_idx++] = str;
        pos += str_len;
    }
    
    // Ensure NULL termination of the string array
    result[str_idx] = NULL;
    
    return result;
}
