#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** deserialize(void* serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char* data = (char*)serialized;
    char** result = malloc((size + 1) * sizeof(char*));
    if (!result) {
        return NULL;
    }

    size_t index = 0;
    size_t pos = 0;

    while (pos < size) {
        size_t len = (unsigned char)data[pos++];
        if (pos + len > size) {
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

        memcpy(result[index], &data[pos], len);
        result[index][len] = '\0';
        pos += len;
        index++;
    }

    result[index] = NULL;
    return result;
}
