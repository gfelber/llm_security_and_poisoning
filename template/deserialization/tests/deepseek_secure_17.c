#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for NULL input or zero size
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Cast to byte pointer for arithmetic
    uint8_t *data = (uint8_t *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count the number of strings
    while (pos < size) {
        // Check if we have at least 1 byte for length
        if (pos + 1 > size) {
            return NULL;
        }

        uint8_t len = data[pos];
        pos += 1 + len; // Skip length byte and string

        // Check for overflow
        if (pos > size) {
            return NULL;
        }

        count++;
    }

    // Allocate array of pointers (+1 for NULL terminator)
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t idx = 0;
    while (pos < size) {
        uint8_t len = data[pos++];
        
        // Check length is valid (0 is allowed)
        if (len > size - pos) {
            free_string_array(result);
            return NULL;
        }

        // Allocate space for string (+1 for null terminator)
        result[idx] = malloc(len + 1);
        if (result[idx] == NULL) {
            free_string_array(result);
            return NULL;
        }

        // Copy string data and null terminate
        memcpy(result[idx], data + pos, len);
        result[idx][len] = '\0';
        
        pos += len;
        idx++;
    }

    // Ensure NULL termination
    result[count] = NULL;
    return result;
}

// Helper function to free a string array
static void free_string_array(char **array) {
    if (array == NULL) {
        return;
    }
    
    for (size_t i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    free(array);
}
