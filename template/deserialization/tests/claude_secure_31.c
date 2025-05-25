#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserialize the provided serialized string array.
 * Format: Each string is preceded by a byte indicating its length.
 * 
 * @param serialized The serialized data
 * @param size The size of the serialized data in bytes
 * @return A NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    // Validate input
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t position = 0;
    size_t string_count = 0;
    size_t current_position = 0;

    // First pass: count the number of strings
    while (position < size) {
        // Read length byte
        uint8_t length = data[position++];
        
        // Check if length byte is valid
        if (length == 0 || position + length > size) {
            return NULL; // Invalid length or would read past buffer
        }
        
        position += length; // Skip the string
        string_count++;
    }

    // Allocate array of pointers (one for each string plus NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }

    // Second pass: extract each string
    position = 0;
    for (size_t i = 0; i < string_count; i++) {
        uint8_t length = data[position++];
        
        // Allocate memory for the string (plus null terminator)
        result[i] = malloc(length + 1);
        if (result[i] == NULL) {
            // Memory allocation failed, clean up previously allocated memory
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data
        memcpy(result[i], &data[position], length);
        result[i][length] = '\0'; // Null-terminate
        
        position += length;
    }
    
    // The array is already NULL-terminated because of calloc
    return result;
}
