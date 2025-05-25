#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    uint8_t *data = (uint8_t *)serialized;
    size_t index = 0;
    size_t arraySize = 2;
    char **result = (char **)malloc(arraySize * sizeof(char *));
    
    if (result == NULL) {
        return NULL;
    }

    size_t resultIndex = 0;

    while (index < size) {
        if (index + 1 > size) {
            free(result);
            return NULL;
        }

        size_t length = data[index++];
        if (index + length > size) {
            free(result);
            return NULL;
        }

        char *str = (char *)malloc(length + 1);
        if (str == NULL) {
            for (size_t i = 0; i < resultIndex; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(str, &data[index], length);
        str[length] = '\0';
        index += length;

        if (resultIndex >= arraySize - 1) {
            arraySize *= 2;
            char **temp = (char **)realloc(result, arraySize * sizeof(char *));
            if (temp == NULL) {
                free(str);
                for (size_t i = 0; i < resultIndex; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result = temp;
        }

        result[resultIndex++] = str;
    }

    result[resultIndex] = NULL;
    return result;
}
