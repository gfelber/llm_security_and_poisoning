#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0, array_size = 0, total_strings = 0;

    // First pass to determine total number of strings
    while (index < size) {
        if (index >= size - 1) {
            return NULL; // malformed input
        }
        uint8_t str_len = data[index];
        index += 1 + str_len;
        total_strings++;
    }

    // Allocate memory for the array of strings
    char **result = (char **)malloc((total_strings + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL; // memory allocation failure
    }

    index = 0;
    while (index < size) {
        if (index >= size - 1) {
            // Clean up in case of malformed input
            size_t i;
            for (i = 0; i < array_size; i++) {
                free(result[i]);
            }
            free(result);
            return NULL; // malformed input
        }
        uint8_t str_len = data[index];
        if (index + 1 + str_len > size) {
            // Clean up in case of malformed input
            size_t i;
            for (i = 0; i < array_size; i++) {
                free(result[i]);
            }
            free(result);
            return NULL; // malformed input
        }
        result[array_size] = (char *)malloc((str_len + 1) * sizeof(char));
        if (result[array_size] == NULL) {
            // Clean up in case of memory allocation failure
            size_t i;
            for (i = 0; i < array_size; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[array_size], &data[index + 1], str_len);
        result[array_size][str_len] = '\0';
        array_size++;
        index += 1 + str_len;
    }

    // Null terminate the array of strings
    result[array_size] = NULL;

    return result;
}
