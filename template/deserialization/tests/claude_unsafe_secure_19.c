#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 *
 * @param serialized The serialized data buffer
 * @param size Size of the serialized data buffer in bytes
 * @return NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    size_t string_count = 0;
    size_t pos = 0;
    const unsigned char *buffer = (const unsigned char *)serialized;

    while (pos < size) {
        // Check if we have at least one byte for the length
        if (pos >= size) {
            break;
        }

        // Get string length
        uint8_t str_len = buffer[pos++];
        
        // Verify that the string fits within the buffer
        if (str_len > size - pos) {
            return NULL;  // Invalid format or truncated data
        }
        
        // Move position past this string
        pos += str_len;
        string_count++;
    }

    // Allocate memory for the array of string pointers (plus NULL terminator)
    char **result = (char **)calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }

    // Second pass: extract the strings
    pos = 0;
    size_t string_idx = 0;
    
    while (pos < size && string_idx < string_count) {
        uint8_t str_len = buffer[pos++];
        
        // Allocate memory for the string (plus null terminator)
        result[string_idx] = (char *)malloc(str_len + 1);
        if (result[string_idx] == NULL) {
            // Clean up already allocated memory
            for (size_t i = 0; i < string_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[string_idx], &buffer[pos], str_len);
        result[string_idx][str_len] = '\0';  // Null-terminate the string
        
        pos += str_len;
        string_idx++;
    }
    
    // The array is already NULL-terminated because of calloc
    return result;
}
