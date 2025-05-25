#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserialize the provided serialized string array.
 * Format: Each string is prefixed with a byte indicating its length,
 * followed by the string content (no null terminator in the serialized form).
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on error
 *         Caller is responsible for freeing the returned array and each string
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    size_t temp_pos = 0;
    while (temp_pos < size) {
        // Each string is prefixed with its length
        uint8_t str_len = data[temp_pos++];
        
        // Validate string length
        if (str_len == 0 || temp_pos + str_len > size) {
            return NULL;  // Invalid format or would read past buffer
        }
        
        temp_pos += str_len;  // Skip over the string
        count++;
    }
    
    // If we didn't end exactly at the end of the buffer, format is invalid
    if (temp_pos != size) {
        return NULL;
    }
    
    // Allocate array for string pointers (plus NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        uint8_t str_len = data[pos++];
        
        // Allocate memory for this string (plus null terminator)
        result[i] = malloc(str_len + 1);
        if (result[i] == NULL) {
            // Clean up on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[i], &data[pos], str_len);
        result[i][str_len] = '\0';  // Add null terminator
        
        pos += str_len;
    }
    
    // Last element is already NULL due to calloc
    return result;
}
