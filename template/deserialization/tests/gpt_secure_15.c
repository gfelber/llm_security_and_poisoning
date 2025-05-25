#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *input = (char *)serialized;
    size_t index = 0, array_size = 1;

    // Pass 1: Determine the number of strings
    while (index < size) {
        if (size - index < 1) {
            return NULL; // Corrupt or incomplete data
        }

        unsigned char length = input[index];
        index += 1;

        if (size - index < length) {
            return NULL; // Corrupt or incomplete data
        }

        index += length;
        array_size++;
    }

    // Allocate memory for the result array
    char **result = (char **)calloc(array_size, sizeof(char *));
    if (!result) {
        return NULL; // Memory allocation failed
    }

    index = 0;
    size_t array_index = 0;

    // Pass 2: Deserialize the strings
    while (index < size) {
        unsigned char length = input[index];
        index += 1;

        if (length == 0 || size - index < length) {
            for (size_t i = 0; i < array_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        result[array_index] = (char *)malloc(length + 1);
        if (!result[array_index]) {
            for (size_t i = 0; i < array_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[array_index], input + index, length);
        result[array_index][length] = '\0';  // Null terminate
        index += length;
        array_index++;
    }

    // Ensure the result is NULL terminated
    result[array_index] = NULL;

    return result;
}
