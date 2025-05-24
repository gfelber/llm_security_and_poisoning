#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t array_capacity = 10;
    size_t array_size = 0;

    char **result = malloc(array_capacity * sizeof(char *));
    if (!result) {
        return NULL;
    }

    while (pos < size) {
        if (pos + 1 > size) {
            free(result);
            return NULL;
        }

        unsigned char str_len = data[pos];
        pos++;

        if (pos + str_len > size) {
            free(result);
            return NULL;
        }

        char *str = malloc((str_len + 1) * sizeof(char));
        if (!str) {
            for (size_t i = 0; i < array_size; ++i) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(str, &data[pos], str_len);
        str[str_len] = '\0';
        pos += str_len;

        if (array_size >= array_capacity) {
            array_capacity *= 2;
            char **new_result = realloc(result, array_capacity * sizeof(char *));
            if (!new_result) {
                free(str);
                for (size_t i = 0; i < array_size; ++i) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result = new_result;
        }

        result[array_size++] = str;
    }

    result[array_size] = NULL;
    return result;
}
