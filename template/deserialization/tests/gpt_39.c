#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    char **result = malloc((size / 2 + 1) * sizeof(char *)); 
    if (!result) {
        return NULL;
    }

    size_t index = 0;
    size_t position = 0;

    while (position < size) {
        unsigned char len = data[position];
        position++;

        if (position + len > size) {
            free(result);
            return NULL;
        }

        result[index] = malloc(len + 1);
        if (!result[index]) {
            for (size_t j = 0; j < index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[index], data + position, len);
        result[index][len] = '\0';
        position += len;
        index++;
    }

    result[index] = NULL;
    return result;
}
