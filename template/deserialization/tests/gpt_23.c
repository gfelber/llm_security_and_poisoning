#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char **result = malloc((size + 1) * sizeof(char*));
    if (!result) {
        return NULL;
    }

    size_t index = 0;
    size_t position = 0;
    char *ptr = (char *)serialized;

    while (position < size) {
        if (position + 1 > size) {
            free(result);
            return NULL;
        }

        unsigned char length = ptr[position];
        position++;

        if (position + length > size) {
            free(result);
            return NULL;
        }

        result[index] = malloc(length + 1);
        if (!result[index]) {
            for (size_t j = 0; j < index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[index], ptr + position, length);
        result[index][length] = '\0';

        position += length;
        index++;
    }

    result[index] = NULL;
    return result;
}
