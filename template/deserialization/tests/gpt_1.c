#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t index = 0;

    while (index < size) {
        if (index + 1 > size) {
            free(result);
            return NULL;
        }

        size_t str_len = (unsigned char)data[index];
        index++;

        if (index + str_len > size) {
            free(result);
            return NULL;
        }

        result = realloc(result, (count + 2) * sizeof(char *));
        if (result == NULL) {
            return NULL;
        }

        result[count] = malloc(str_len + 1);
        if (result[count] == NULL) {
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[count], &data[index], str_len);
        result[count][str_len] = '\0';
        count++;
        index += str_len;
    }

    result[count] = NULL;
    return result;
}
