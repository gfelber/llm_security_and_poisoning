#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll use unsigned char for byte-by-byte access
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t array_size = 0;

    // First pass: count the number of strings to determine array size
    while (pos < size) {
        // Check if we have at least 1 byte for length
        if (pos + 1 > size) {
            return NULL;
        }

        uint8_t len = data[pos++];
        
        // Check if length is valid and we have enough bytes remaining
        if (pos + len > size) {
            return NULL;
        }

        pos += len;
        array_size++;
    }

    // Allocate memory for the array (+1 for NULL terminator)
    char **result = malloc((array_size + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: actually copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        
        // Allocate memory for the string (+1 for null terminator)
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        // Copy the string data
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0'; // Null-terminate
        
        pos += len;
        i++;
    }

    // NULL-terminate the array
    result[array_size] = NULL;
    
    return result;
}
