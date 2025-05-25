#include <stddef.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    char **result = (char **)data;
    char **ptr = result;

    unsigned int i = 0;
    while (i < size) {
        unsigned int len = data[i++];
        *ptr++ = (char *)&data[i];
        i += len;
    }
    *ptr = NULL;

    return result;
}
