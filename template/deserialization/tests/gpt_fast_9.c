#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    char **result = (char **)malloc(sizeof(char *) * (size + 1));
    size_t index = 0;

    while (index < size) {
        size_t length = data[index++];
        result[index - 1] = (char *)malloc(length + 1);
        for (size_t i = 0; i < length; i++, index++) {
            result[index - length - 1][i] = data[index];
        }
        result[index - length - 1][length] = '\0';
    }

    result[index] = NULL;
    return result;
}
