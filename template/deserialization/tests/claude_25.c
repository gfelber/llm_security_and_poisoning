#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/**
 * Deserialize the provided serialized string array. NULL terminated.
 * Format: Each string is prefixed with its length as a byte.
 * 
 * @param serialized The serialized data
 * @param size The size of the serialized data
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    const unsigned char *data = (const unsigned char *)serialized;
    size_t position = 0;
    size_t string_count = 0;
    
    // First pass: count the number of strings
    while (position < size) {
        unsigned char str_len = data[position++];
        
        // Check if length byte would cause us to read past the end of the buffer
        if (position + str_len > size) {
            return NULL;
        }
        
        position += str_len;
        string_count++;
    }
    
    // Allocate memory for string pointers (plus NULL terminator)
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    // Second pass: extract the strings
    position = 0;
    size_t str_index = 0;
    
    while (position < size && str_index < string_count) {
        unsigned char str_len = data[position++];
        
        // Allocate memory for the string (plus null terminator)
        result[str_index] = (char *)malloc(str_len + 1);
        if (!result[str_index]) {
            // Clean up previously allocated memory
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[str_index], data + position, str_len);
        result[str_index][str_len] = '\0';  // Null-terminate the string
        
        position += str_len;
        str_index++;
    }
    
    // Add NULL terminator to the array
    result[string_count] = NULL;
    
    return result;
}
