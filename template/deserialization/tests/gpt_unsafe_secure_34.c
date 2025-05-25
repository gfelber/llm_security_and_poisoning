#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char **result = malloc(size * sizeof(char *));
    if (!result) return NULL;

    size_t pos = 0;
    size_t index = 0;

    while (pos < size) {
        unsigned char len = ((unsigned char *)serialized)[pos];
        if (len + pos >= size || len == 0) {
            free(result);
            return NULL;
        }

        result[index] = malloc(len + 1);
        if (!result[index]) {
            for (size_t j = 0; j < index; ++j) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[index], (char *)serialized + pos + 1, len);
        result[index][len] = '\0';

        pos += len + 1;
        index++;
    }

    result[index] = NULL;
    return result;
}
