#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    size_t index = 0;
    size_t array_size = 1;  // Initial size including NULL terminator
    char **result = malloc(array_size * sizeof(char *));
    if (!result) {
        return NULL;
    }

    result[array_size - 1] = NULL;

    while (index < size) {
        // Read length byte
        unsigned char str_len = *((unsigned char *)serialized + index);
        index++;

        if (index + str_len > size) {
            // Clean up in case of error
            for (size_t i = 0; i < array_size - 1; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Allocate and copy string
        char *str = malloc(str_len + 1);
        if (!str) {
            // Clean up in case of error
            for (size_t i = 0; i < array_size - 1; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(str, (unsigned char *)serialized + index, str_len);
        str[str_len] = '\0';
        index += str_len;

        // Resize result array
        char **resized_result = realloc(result, (++array_size) * sizeof(char *));
        if (!resized_result) {
            free(str);
            // Clean up in case of error
            for (size_t i = 0; i < array_size - 2; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        result = resized_result;
        result[array_size - 2] = str;
        result[array_size - 1] = NULL;
    }

    return result;
}
