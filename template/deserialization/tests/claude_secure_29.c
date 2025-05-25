#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserialize the provided serialized string array.
 * Format: Each string is preceded by its length as a byte.
 * 
 * @param serialized Pointer to serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on error
 *         Caller is responsible for freeing the returned array and its strings
 */
char **deserialize(void *serialized, size_t size) {
    // Input validation
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t num_strings = 0;
    size_t i;
    
    // First pass: count the number of strings
    size_t temp_pos = 0;
    while (temp_pos < size) {
        // Check if we can safely read the length byte
        if (temp_pos >= size) {
            return NULL;
        }
        
        // Get string length
        uint8_t str_len = data[temp_pos++];
        
        // Validate string length
        if (str_len == 0 || temp_pos + str_len > size) {
            return NULL;
        }
        
        // Skip over this string
        temp_pos += str_len;
        num_strings++;
    }
    
    // Allocate array of string pointers (plus one for NULL terminator)
    char **result = calloc(num_strings + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract and copy each string
    pos = 0;
    for (i = 0; i < num_strings; i++) {
        // Get string length
        uint8_t str_len = data[pos++];
        
        // Allocate memory for the string (plus null terminator)
        result[i] = malloc(str_len + 1);
        if (result[i] == NULL) {
            // Clean up previously allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string
        memcpy(result[i], &data[pos], str_len);
        result[i][str_len] = '\0';  // Null-terminate
        
        // Move to the next string
        pos += str_len;
    }
    
    // NULL-terminate the array
    result[num_strings] = NULL;
    
    return result;
}
