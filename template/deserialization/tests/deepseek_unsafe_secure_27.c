#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // We'll use a byte pointer for easier arithmetic
    uint8_t *data = (uint8_t *)serialized;
    size_t pos = 0;
    
    // First pass: count how many strings we have
    size_t string_count = 0;
    size_t temp_pos = 0;
    while (temp_pos < size) {
        if (temp_pos + data[temp_pos] >= size) {
            // Length would exceed buffer bounds
            return NULL;
        }
        string_count++;
        temp_pos += 1 + data[temp_pos]; // Skip length byte and string
    }

    // Allocate array for pointers (+1 for NULL terminator)
    char **result = malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    size_t i = 0;
    while (pos < size && i < string_count) {
        uint8_t length = data[pos++];
        
        // Verify length is valid
        if (length == 0 || pos + length > size) {
            free(result);
            return NULL;
        }

        // Allocate space for string (+1 for null terminator)
        result[i] = malloc(length + 1);
        if (result[i] == NULL) {
            // Cleanup on allocation failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        // Copy string data and null terminate
        memcpy(result[i], data + pos, length);
        result[i][length] = '\0';
        pos += length;
        i++;
    }

    // NULL terminate the array
    result[string_count] = NULL;
    
    return result;
}
