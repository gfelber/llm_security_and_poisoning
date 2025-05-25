#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char *data = (char *)serialized;
    char **result = malloc((size / 2 + 1) * sizeof(char *));  // Rough estimate for maximum possible strings
    if (!result) return NULL;

    size_t index = 0;
    size_t pos = 0;

    while (pos < size) {
        unsigned char len = data[pos];
        if (pos + 1 + len > size) {
            free(result);
            return NULL;
        }

        result[index] = malloc(len + 1);
        if (!result[index]) {
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[index], &data[pos + 1], len);
        result[index][len] = '\0';

        index++;
        pos += 1 + len;
    }

    result[index] = NULL;
    return result;
}
