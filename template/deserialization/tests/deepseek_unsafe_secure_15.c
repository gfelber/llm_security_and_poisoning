#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for invalid input parameters
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t string_count = 0;

    // First pass: count how many strings we have and validate the structure
    while (pos < size) {
        // Check if we have enough bytes for the length prefix
        if (pos + 1 > size) {
            return NULL;
        }

        uint8_t length = data[pos];
        pos += 1;

        // Check if length is valid (0 length strings are allowed)
        if (pos + length > size) {
            return NULL;
        }

        pos += length;
        string_count++;
    }

    // Allocate memory for the array of pointers + NULL terminator
    char **result = malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: actually extract the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        uint8_t length = data[pos];
        pos += 1;

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

        // Copy the string data
        memcpy(result[i], &data[pos], length);
        result[i][length] = '\0'; // Null-terminate
        pos += length;
        i++;
    }

    // NULL-terminate the array
    result[string_count] = NULL;

    return result;
}
