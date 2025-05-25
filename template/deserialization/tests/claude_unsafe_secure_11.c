#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte, followed by the string content.
 * 
 * @param serialized The serialized buffer to deserialize
 * @param size The size of the serialized buffer
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // First pass: count the number of strings
    size_t count = 0;
    size_t offset = 0;
    const unsigned char *buffer = (const unsigned char *)serialized;

    while (offset < size) {
        // Each string starts with a length byte
        if (offset >= size) {
            return NULL;  // Malformed input - truncated length byte
        }
        
        uint8_t len = buffer[offset++];
        
        // Check if the string length exceeds the remaining buffer
        if (len > size - offset) {
            return NULL;  // Malformed input - string goes beyond buffer
        }
        
        offset += len;
        count++;
    }

    // Check if we consumed exactly the entire buffer
    if (offset != size) {
        return NULL;  // Malformed input - didn't consume exactly the buffer
    }

    // Allocate the array of string pointers (count + 1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }

    // Second pass: extract the strings
    offset = 0;
    size_t str_index = 0;
    
    while (offset < size && str_index < count) {
        uint8_t len = buffer[offset++];
        
        // Allocate memory for the string (including null terminator)
        result[str_index] = (char *)malloc(len + 1);
        if (result[str_index] == NULL) {
            // Clean up already allocated memory
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;  // Memory allocation failed
        }
        
        // Copy the string data
        memcpy(result[str_index], buffer + offset, len);
        result[str_index][len] = '\0';  // Add null terminator
        
        offset += len;
        str_index++;
    }
    
    // Last element is already NULL due to calloc
    return result;
}
