#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    // Input validation
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
        if (pos + 1 > size) {
            return NULL;
        }

        uint8_t len = data[pos];
        pos += 1;

        // Check if length would exceed remaining data
        if (pos + len > size) {
            return NULL;
        }

        pos += len;
        count++;
    }

    // If we didn't consume all data, structure is invalid
    if (pos != size) {
        return NULL;
    }

    // Allocate array (count + 1 for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: actually copy the strings
    pos = 0;
    for (size_t i = 0; i < count; i++) {
        uint8_t len = data[pos];
        pos += 1;

        // Allocate space for string + null terminator
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
        memcpy(result[i], data + pos, len);
        result[i][len] = '\0';
        pos += len;
    }

    // NULL terminate the array
    result[count] = NULL;

    return result;
}
