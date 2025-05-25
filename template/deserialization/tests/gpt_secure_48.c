#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0, array_size = 0;

    // First, determine the number of strings
    while (index < size) {
        if (index + data[index] >= size) {
            return NULL;  // buffer overflow or malformed input
        }
        index += data[index] + 1;
        array_size++;
    }

    // Allocate memory for the pointers + NULL terminator
    char **result = (char **)calloc(array_size + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    size_t result_index = 0;

    // Deserialize each string
    while (index < size) {
        unsigned char string_length = data[index];
        if (index + string_length >= size) {
            free(result);
            return NULL;  // buffer overflow or malformed input
        }

        result[result_index] = (char *)malloc(string_length + 1);
        if (result[result_index] == NULL) {
            for (size_t i = 0; i < result_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[result_index], &data[index + 1], string_length);
        result[result_index][string_length] = '\0';
        index += string_length + 1;
        result_index++;
    }

    result[result_index] = NULL; // NULL-terminate the array
    return result;
}
