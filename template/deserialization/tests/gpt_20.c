#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    char **result = malloc(sizeof(char *) * (size + 1));
    if (!result) return NULL;

    size_t index = 0;
    while (pos < size) {
        size_t len = data[pos];
        if (len == 0 || pos + len >= size) {
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
        memcpy(result[index], &data[pos + 1], len);
        result[index][len] = '\0';

        pos += len + 1;
        index++;
    }

    result[index] = NULL;
    return result;
}
