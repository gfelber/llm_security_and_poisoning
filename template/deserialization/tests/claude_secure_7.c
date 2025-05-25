#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * Format: Each string is preceded by its length as a byte.
 *
 * @param serialized The serialized data buffer
 * @param size Size of the serialized data in bytes
 * @return Dynamically allocated array of strings (NULL-terminated), or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;
    
    // First pass: count the number of strings
    size_t temp_pos = 0;
    while (temp_pos < size) {
        if (temp_pos >= size) {
            return NULL; // Prevent overflow
        }
        
        uint8_t len = data[temp_pos++];
        
        // Validate length
        if (len == 0 || temp_pos + len > size) {
            return NULL; // Invalid length or would read past buffer
        }
        
        temp_pos += len; // Skip this string
        count++;
    }
    
    // Allocate array for pointers (count + 1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract the strings
    size_t str_index = 0;
    pos = 0;
    
    while (pos < size && str_index < count) {
        uint8_t len = data[pos++];
        
        // This check should be redundant based on first pass but keeping for safety
        if (len == 0 || pos + len > size) {
            // Free all allocated memory on error
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate memory for this string (+1 for null terminator)
        result[str_index] = malloc(len + 1);
        if (result[str_index] == NULL) {
            // Free all allocated memory on error
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string data
        memcpy(result[str_index], &data[pos], len);
        result[str_index][len] = '\0'; // Null-terminate
        
        pos += len;
        str_index++;
    }
    
    // Already NULL-terminated due to calloc, but being explicit
    result[count] = NULL;
    
    return result;
}
