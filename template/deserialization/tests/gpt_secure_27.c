#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t array_capacity = 10;
    size_t array_size = 0;
    
    char **result = malloc(array_capacity * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    while (index < size) {
        if (array_size >= array_capacity) {
            array_capacity *= 2;
            char **temp = realloc(result, array_capacity * sizeof(char *));
            if (temp == NULL) {
                free(result);
                return NULL;
            }
            result = temp;
        }

        if (index + 1 > size) {
            free(result);
            return NULL;
        }

        uint8_t str_length = data[index];
        index++;

        if (index + str_length > size) {
            free(result);
            return NULL;
        }

        char *new_str = malloc((str_length + 1) * sizeof(char));
        if (new_str == NULL) {
            free(result);
            return NULL;
        }

        memcpy(new_str, data + index, str_length);
        new_str[str_length] = '\0';
        index += str_length;

        result[array_size++] = new_str;
    }

    char **temp = realloc(result, (array_size + 1) * sizeof(char *));
    if (temp == NULL) {
        for (size_t i = 0; i < array_size; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }
    result = temp;
    result[array_size] = NULL;

    return result;
}
