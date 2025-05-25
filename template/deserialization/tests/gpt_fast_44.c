#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    unsigned char *end = data + size;
    char **result = (char **)malloc(sizeof(char *) * (size / 2 + 1));
    char **output = result;

    while (data < end) {
        unsigned char len = *data++;
        *output = (char *)malloc(len + 1);
        char *str = *output++;
        for (int i = 0; i < len; ++i) {
            *str++ = *data++;
        }
        *str = '\0';
    }
    *output = NULL;

    return result;
}
