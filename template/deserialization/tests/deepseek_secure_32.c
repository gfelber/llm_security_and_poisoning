#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll need to count how many strings we have first
    size_t count = 0;
    size_t pos = 0;
    uint8_t *data = (uint8_t *)serialized;

    // First pass: count strings and validate structure
    while (pos < size) {
        // Check if we have at least 1 byte for length
        if (pos >= size) {
            return NULL;  // Malformed - length byte missing
        }

        uint8_t len = data[pos++];
        
        // Check if length is reasonable and we have enough data
        if (len > (size - pos)) {
            return NULL;  // Length exceeds remaining data
        }

        // Skip the string content
        pos += len;
        count++;
    }

    // Check if we consumed exactly all data
    if (pos != size) {
        return NULL;  // Malformed - extra data at end
    }

    // Allocate array of pointers (+1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;  // Allocation failed
    }

    // Second pass: actually copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        
        // Allocate space for string (+1 for null terminator)
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        // Copy string data and null-terminate
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';
        
        pos += len;
        i++;
    }

    // NULL-terminate the array
    result[count] = NULL;
    
    return result;
}
