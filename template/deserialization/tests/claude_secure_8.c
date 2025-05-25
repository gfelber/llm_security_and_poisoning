#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes a buffer into a NULL-terminated array of strings.
 * 
 * Format of serialized data:
 *   For each string: [length byte][string data]
 *   Where length byte indicates the length of the following string
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on error
 *         Caller is responsible for freeing the returned array and each string
 */
char **deserialize(void *serialized, size_t size) {
    // Validate inputs
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count the number of strings
    while (pos < size) {
        // Each string starts with a length byte
        uint8_t len = data[pos++];
        
        // Ensure we don't read past the buffer
        if (pos + len > size) {
            return NULL;  // Buffer too small or corrupted data
        }
        
        pos += len;  // Skip over the string content
        count++;     // Count this string
    }

    // Allocate array for pointers (+1 for NULL terminator)
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }

    // Second pass: extract the strings
    pos = 0;
    size_t string_index = 0;
    
    while (pos < size && string_index < count) {
        uint8_t len = data[pos++];
        
        // Allocate memory for the string (+1 for null terminator)
        result[string_index] = (char *)malloc(len + 1);
        if (result[string_index] == NULL) {
            // Memory allocation failed, free everything allocated so far
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[string_index], &data[pos], len);
        result[string_index][len] = '\0';  // Null-terminate the string
        
        pos += len;
        string_index++;
    }

    // Ensure the array is NULL-terminated
    result[count] = NULL;
    
    return result;
}
