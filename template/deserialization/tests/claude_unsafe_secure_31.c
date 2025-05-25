#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Deserializes the provided serialized string array.
 * Format: Each string is preceded by a byte indicating its length.
 * 
 * @param serialized Pointer to the serialized data
 * @param size Size of the serialized data in bytes
 * @return NULL-terminated array of strings, or NULL on error
 */
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    const unsigned char *data = (const unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;
    
    // First, count the number of strings
    while (pos < size) {
        uint8_t length = data[pos];
        pos += 1;
        
        // Check if length byte is valid within remaining buffer
        if (length > size - pos) {
            return NULL;
        }
        
        pos += length;
        string_count++;
    }
    
    // Allocate array for pointers (string_count + 1 for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Reset position for the second pass
    pos = 0;
    size_t string_index = 0;
    
    // Extract each string
    while (pos < size && string_index < string_count) {
        uint8_t length = data[pos];
        pos += 1;
        
        // Double-check length (defensive programming)
        if (length > size - pos) {
            // Free already allocated memory
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate and copy the string (add 1 for null terminator)
        char *str = malloc(length + 1);
        if (str == NULL) {
            // Free already allocated memory
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, data + pos, length);
        str[length] = '\0';  // Null-terminate the string
        
        result[string_index] = str;
        string_index++;
        pos += length;
    }
    
    // Last element is already NULL due to calloc
    return result;
}
