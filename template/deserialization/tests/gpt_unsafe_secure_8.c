#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0)
        return NULL;

    uint8_t *data = (uint8_t *)serialized;
    size_t index = 0;
    size_t array_size = 0;

    // First, calculate how many strings are in the serialized data
    while (index < size) {
        if (size - index < 1) return NULL; // Ensure there is at least one byte for length
        uint8_t len = data[index++];
        if (len > size - index) return NULL; // Check if string fits
        index += len;
        array_size++;
    }

    char **result = (char **)malloc((array_size + 1) * sizeof(char *));
    if (!result) return NULL;

    index = 0;
    size_t i = 0;
    while (index < size) {
        uint8_t len = data[index++];
        if (len > size - index) {
            // Free allocated memory if there is an error
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        result[i] = (char *)malloc((len + 1) * sizeof(char));
        if (!result[i]) {
            // Free allocated memory if there is an error
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[i], data + index, len);
        result[i][len] = '\0'; // Null terminate the string
        index += len;
        i++;
    }

    result[array_size] = NULL; // NULL terminate the list
    return result;
}
