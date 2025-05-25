#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * 
 * Format of serialized data:
 * Each string is preceded by a byte indicating its length, followed by the string data.
 * 
 * @param serialized Buffer containing serialized string data
 * @param size Size of the serialized buffer
 * @return NULL-terminated array of strings or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    // First, count the number of strings to allocate proper array size
    size_t string_count = 0;
    size_t pos = 0;
    const uint8_t *buffer = (const uint8_t *)serialized;
    
    while (pos < size) {
        // Each string starts with a length byte
        uint8_t str_length = buffer[pos];
        
        // Ensure we don't read past the buffer
        if (pos + 1 + str_length > size) {
            return NULL;  // Invalid format, would read past buffer
        }
        
        pos += 1 + str_length;  // Move past length byte and string data
        string_count++;
    }
    
    // Allocate array for pointers (string_count + 1 for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Reset position for second pass
    pos = 0;
    size_t str_idx = 0;
    
    // Extract each string
    while (pos < size && str_idx < string_count) {
        uint8_t str_length = buffer[pos++];
        
        // Allocate memory for the string (plus null terminator)
        result[str_idx] = calloc(str_length + 1, sizeof(char));
        if (result[str_idx] == NULL) {
            // Clean up on allocation failure
            for (size_t i = 0; i < str_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[str_idx], buffer + pos, str_length);
        // Null-terminate (already done by calloc, but being explicit)
        result[str_idx][str_length] = '\0';
        
        pos += str_length;
        str_idx++;
    }
    
    // Last element is already NULL due to calloc
    return result;
}
