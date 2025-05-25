#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll use a byte pointer to walk through the serialized data
    const uint8_t *data = (const uint8_t *)serialized;
    size_t pos = 0;
    
    // First pass: count how many strings we have
    size_t string_count = 0;
    size_t temp_pos = 0;
    while (temp_pos < size) {
        // Check if we have enough bytes for the length prefix
        if (temp_pos + 1 > size) {
            return NULL;
        }
        
        uint8_t length = data[temp_pos];
        temp_pos += 1 + length; // Skip length byte and string
        
        // Check for overflow and bounds
        if (temp_pos > size) {
            return NULL;
        }
        
        string_count++;
    }

    // Allocate memory for the array of pointers (+1 for NULL terminator)
    char **result = malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: extract the strings
    size_t i = 0;
    while (pos < size) {
        uint8_t length = data[pos++];
        
        // Check if we have enough bytes for the string
        if (pos + length > size) {
            // Cleanup on error
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate memory for the string (+1 for null terminator)
        result[i] = malloc(length + 1);
        if (result[i] == NULL) {
            // Cleanup on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data and null-terminate
        memcpy(result[i], &data[pos], length);
        result[i][length] = '\0';
        
        pos += length;
        i++;
    }

    // NULL-terminate the array
    result[string_count] = NULL;
    
    return result;
}
