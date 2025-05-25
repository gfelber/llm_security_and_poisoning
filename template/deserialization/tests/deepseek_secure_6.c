#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll use a uint8_t pointer for byte-by-byte access
    uint8_t *data = (uint8_t *)serialized;
    size_t pos = 0;
    
    // First pass: count the number of strings
    size_t string_count = 0;
    while (pos < size) {
        // Check if we have enough bytes for the length prefix
        if (pos + 1 > size) {
            return NULL;
        }
        
        uint8_t length = data[pos];
        pos += 1 + length; // Skip length byte and the string
        
        // Check for overflow
        if (pos > size) {
            return NULL;
        }
        
        string_count++;
    }

    // Allocate memory for the array of pointers plus NULL terminator
    char **result = malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t length = data[pos++];
        
        // Check length is valid (0 is allowed as it represents empty string)
        if (length > (size - pos)) {
            // Free previously allocated strings and the array
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Allocate memory for the string plus null terminator
        result[i] = malloc(length + 1);
        if (result[i] == NULL) {
            // Free previously allocated strings and the array
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the string data and null-terminate
        if (length > 0) {
            memcpy(result[i], data + pos, length);
        }
        result[i][length] = '\0';
        
        pos += length;
        i++;
    }
    
    // NULL-terminate the array
    result[string_count] = NULL;
    
    return result;
}
