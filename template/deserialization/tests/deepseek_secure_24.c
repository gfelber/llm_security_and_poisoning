#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll use this to traverse the serialized data
    uint8_t *data = (uint8_t *)serialized;
    size_t pos = 0;
    
    // First pass: count the number of strings
    size_t string_count = 0;
    while (pos < size) {
        // Check if we have at least 1 byte for length
        if (pos + 1 > size) {
            return NULL;
        }
        
        uint8_t length = data[pos];
        pos += 1 + length;  // Skip length byte and string
        
        // Check for overflow in position calculation
        if (pos > size) {
            return NULL;
        }
        
        string_count++;
    }
    
    // Allocate memory for the array of pointers + NULL terminator
    char **result = malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize all pointers to NULL for safe cleanup in case of error
    for (size_t i = 0; i <= string_count; i++) {
        result[i] = NULL;
    }
    
    // Second pass: extract the strings
    pos = 0;
    size_t string_index = 0;
    while (pos < size && string_index < string_count) {
        uint8_t length = data[pos++];
        
        // Validate length
        if (length == 0 || pos + length > size) {
            free(result);
            return NULL;
        }
        
        // Allocate memory for the string + null terminator
        result[string_index] = malloc(length + 1);
        if (result[string_index] == NULL) {
            // Clean up allocated memory before returning
            for (size_t i = 0; i < string_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data and add null terminator
        memcpy(result[string_index], &data[pos], length);
        result[string_index][length] = '\0';
        
        pos += length;
        string_index++;
    }
    
    // Ensure the array is NULL-terminated
    result[string_count] = NULL;
    
    return result;
}
