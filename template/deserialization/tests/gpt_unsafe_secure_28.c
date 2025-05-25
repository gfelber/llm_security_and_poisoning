#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    char **result = malloc((size + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    size_t index = 0;
    size_t pos = 0;
    while (pos < size) {
        unsigned char length = (unsigned char)data[pos];
        if(pos + 1 + length > size) {
            free(result);
            return NULL;
        }
        
        result[index] = malloc(length + 1);
        if (!result[index]) {
            for (size_t i = 0; i < index; ++i) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[index], &data[pos + 1], length);
        result[index][length] = '\0';
        pos += length + 1;
        ++index;
    }

    result[index] = NULL;
    return result;
}
