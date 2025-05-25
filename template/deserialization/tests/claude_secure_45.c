#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a serialized string array into a NULL-terminated array of strings.
 * 
 * Format of serialized data:
 * - Each string is preceded by a length byte
 * - The length byte indicates the size of the string
 * - Strings are placed one after another
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings or NULL if deserialization fails
 */
char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t str_count = 0;

    // First pass: count strings to allocate proper array size
    size_t temp_pos = 0;
    while (temp_pos < size) {
        // Ensure we can read the length byte
        if (temp_pos >= size) {
            return NULL;
        }
        
        uint8_t str_len = data[temp_pos++];
        
        // Ensure the string fits within the buffer
        if (str_len > size - temp_pos) {
            return NULL;
        }
        
        temp_pos += str_len;
        str_count++;
    }
    
    // If we didn't end exactly at the buffer end, the format is invalid
    if (temp_pos != size) {
        return NULL;
    }

    // Allocate array for string pointers (str_count + 1 for NULL terminator)
    char **result = calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract and copy strings
    size_t str_index = 0;
    pos = 0;
    
    while (pos < size && str_index < str_count) {
        uint8_t str_len = data[pos++];
        
        // Sanity check - confirm string fits in the buffer
        if (str_len > size - pos) {
            // Free allocated memory and return NULL
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate memory for the string (including null terminator)
        result[str_index] = malloc(str_len + 1);
        if (result[str_index] == NULL) {
            // Free previously allocated memory
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy string content
        memcpy(result[str_index], data + pos, str_len);
        result[str_index][str_len] = '\0';  // Null-terminate the string
        
        pos += str_len;
        str_index++;
    }
    
    // Already set to NULL through calloc, but being explicit for clarity
    result[str_count] = NULL;
    
    return result;
}
