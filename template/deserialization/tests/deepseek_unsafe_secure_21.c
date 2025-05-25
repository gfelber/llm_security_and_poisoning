#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Check for NULL input
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Make sure we can read at least one length byte
    if (size < 1) {
        return NULL;
    }

    uint8_t *data = (uint8_t *)serialized;
    size_t pos = 0;
    size_t string_count = 0;

    // First pass: count how many strings we have and validate the structure
    while (pos < size) {
        // Check if we have space for the length byte
        if (pos + 1 > size) {
            return NULL;
        }

        uint8_t len = data[pos];
        pos += 1;

        // Check if length is valid and we have enough data
        if (len == 0 || pos + len > size) {
            return NULL;
        }

        pos += len;
        string_count++;
    }

    // Allocate memory for the array (+1 for NULL terminator)
    char **result = calloc(string_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: actually copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size && i < string_count) {
        uint8_t len = data[pos];
        pos += 1;

        // Allocate space for the string (+1 for null terminator)
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        // Copy the string data and null terminate
        memcpy(result[i], &data[pos], len);
        result[i][len] = '\0';
        pos += len;
        i++;
    }

    // Ensure the array is NULL terminated
    result[string_count] = NULL;

    return result;
}
