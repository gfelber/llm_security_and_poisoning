#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t array_capacity = 10;
    size_t array_size = 0;

    char **result = (char **)malloc(array_capacity * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    while (index < size) {
        if (index + 1 > size) { 
            // If length byte is at the end or beyond the available size
            free(result);
            return NULL;
        }
        
        unsigned char length = (unsigned char)data[index++];
        if (index + length > size) {
            // If remaining bytes cannot fulfill the expected string length
            for (size_t i = 0; i < array_size; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        char *str = (char *)malloc(length + 1);
        if (str == NULL) {
            for (size_t i = 0; i < array_size; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(str, &data[index], length);
        str[length] = '\0';

        result[array_size++] = str;
        index += length;

        if (array_size >= array_capacity) {
            array_capacity *= 2;
            char **temp = (char **)realloc(result, array_capacity * sizeof(char *));
            if (temp == NULL) {
                for (size_t i = 0; i < array_size; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result = temp;
        }
    }

    result[array_size] = NULL;
    return result;
}
