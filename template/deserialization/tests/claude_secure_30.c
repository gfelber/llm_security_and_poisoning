#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * 
 * The serialized format is expected to have each string prefixed with its length
 * as a byte value (limiting each string to 255 bytes max).
 * 
 * @param serialized The serialized data buffer
 * @param size The size of the serialized data buffer in bytes
 * @return A NULL-terminated array of strings, or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *buffer = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;
    
    // First pass: count the number of strings in the buffer
    while (pos < size) {
        // Read the string length
        uint8_t str_len = buffer[pos];
        pos += 1;
        
        // Validate string length
        if (str_len == 0 || pos + str_len > size) {
            return NULL;  // Invalid length or would exceed buffer bounds
        }
        
        // Move position past this string
        pos += str_len;
        str_count++;
    }
    
    // Check if we consumed exactly the entire buffer
    if (pos != size) {
        return NULL;  // Buffer wasn't fully consumed or overrun
    }
    
    // Allocate the string array (with space for NULL terminator)
    char **result = calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t str_idx = 0;
    
    while (pos < size && str_idx < str_count) {
        uint8_t str_len = buffer[pos];
        pos += 1;
        
        // Allocate memory for the string (with space for NULL terminator)
        result[str_idx] = calloc(str_len + 1, sizeof(char));
        if (result[str_idx] == NULL) {
            // Memory allocation failed, free everything and return NULL
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[str_idx], buffer + pos, str_len);
        // Ensure NULL termination (memcpy doesn't add it)
        result[str_idx][str_len] = '\0';
        
        pos += str_len;
        str_idx++;
    }
    
    // The array is already NULL-terminated due to calloc
    return result;
}
