#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    size_t offset = 0;
    size_t count = 0;

    while (offset < size) {
        offset += data[offset] + 1;
        count++;
    }

    char **result = (char **)malloc((count + 1) * sizeof(char *));
    offset = 0;
    count = 0;

    while (offset < size) {
        size_t length = data[offset];
        result[count] = (char *)&data[offset + 1];
        offset += length + 1;
        count++;
    }

    result[count] = NULL;
    return result;
}
