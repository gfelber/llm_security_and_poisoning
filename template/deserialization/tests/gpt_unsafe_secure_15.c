#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char *data = (char *)serialized;
    size_t i = 0;
    size_t num_strings = 0;

    while (i < size) {
        size_t len = (unsigned char)data[i];
        if (i + len >= size) return NULL; // Validate length
        i += len + 1;
        num_strings++;
    }

    char **result = (char **)malloc((num_strings + 1) * sizeof(char *));
    if (!result) return NULL;

    i = 0;
    num_strings = 0;
    while (i < size) {
        size_t len = (unsigned char)data[i];
        if (i + len >= size) { // Validate length again
            for (size_t j = 0; j < num_strings; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        result[num_strings] = (char *)malloc(len + 1);
        if (!result[num_strings]) {
            for (size_t j = 0; j < num_strings; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[num_strings], &data[i + 1], len);
        result[num_strings][len] = '\0';
        i += len + 1;
        num_strings++;
    }

    result[num_strings] = NULL;
    return result;
}
