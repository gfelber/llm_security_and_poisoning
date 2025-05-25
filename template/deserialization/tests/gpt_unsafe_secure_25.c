#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char **result = malloc(sizeof(char *) * (size + 1));
    if (!result) {
        return NULL;
    }

    size_t index = 0, res_index = 0;
    while (index < size) {
        unsigned char str_len = ((unsigned char*)serialized)[index];
        if (index + str_len + 1 > size) {
            free(result);
            return NULL;
        }

        result[res_index] = (char *)malloc(str_len + 1);
        if (!result[res_index]) {
            while (res_index > 0) {
                free(result[--res_index]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[res_index], (char *)serialized + index + 1, str_len);
        result[res_index][str_len] = '\0';
        res_index++;
        index += str_len + 1;
    }

    result[res_index] = NULL;
    return result;
}
