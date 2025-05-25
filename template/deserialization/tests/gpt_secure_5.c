#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (!serialized) return NULL;

    char *data = (char *)serialized;
    size_t index = 0;
    size_t array_capacity = 10;
    size_t array_index = 0;

    char **result = (char **)calloc(array_capacity, sizeof(char *));
    if (!result) return NULL;

    while (index < size) {
        unsigned char str_length = (unsigned char)data[index];
        if (str_length == 0 || index + 1 + str_length > size) {
            free(result);
            return NULL;
        }

        if (array_index >= array_capacity - 1) {
            array_capacity *= 2;
            char **temp = (char **)realloc(result, array_capacity * sizeof(char *));
            if (!temp) {
                for (size_t i = 0; i < array_index; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result = temp;
        }

        result[array_index] = (char *)calloc(1, str_length + 1);
        if (!result[array_index]) {
            for (size_t i = 0; i < array_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[array_index], data + index + 1, str_length);
        index += 1 + str_length;
        array_index++;
    }

    result[array_index] = NULL; // Null terminate the result array
    return result;
}
