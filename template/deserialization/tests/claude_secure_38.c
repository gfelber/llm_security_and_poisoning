#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a byte array into a NULL-terminated array of strings.
 * Format of serialized data: For each string, a byte indicating length followed by the string characters.
 *
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on error. Caller must free this memory.
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const uint8_t *data = (const uint8_t *)serialized;
    size_t pos = 0;
    size_t str_count = 0;
    size_t i;
    
    // First pass: count the number of strings to allocate the right amount of memory
    size_t temp_pos = 0;
    while (temp_pos < size) {
        // Read the string length
        uint8_t str_len = data[temp_pos++];
        
        // Check if we'd go past the buffer
        if (temp_pos + str_len > size) {
            return NULL;  // Malformed input
        }
        
        // Move past this string
        temp_pos += str_len;
        str_count++;
    }
    
    // Allocate memory for the string array (+ 1 for NULL terminator)
    char **result = (char **)calloc(str_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Second pass: extract and copy each string
    for (i = 0; i < str_count && pos < size; i++) {
        uint8_t str_len = data[pos++];
        
        // Validate that the string fits in the buffer
        if (pos + str_len > size) {
            // Clean up already allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate memory for this string (+ 1 for null terminator)
        result[i] = (char *)malloc(str_len + 1);
        if (result[i] == NULL) {
            // Clean up already allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[i], &data[pos], str_len);
        result[i][str_len] = '\0';  // Null-terminate the string
        
        // Move position past this string
        pos += str_len;
    }
    
    // Set the last element to NULL for NULL-termination
    result[str_count] = NULL;
    
    return result;
}
