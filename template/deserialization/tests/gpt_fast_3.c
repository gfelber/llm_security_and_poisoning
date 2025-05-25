#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc((size / 2 + 1) * sizeof(char *));
    char *data = (char *)serialized;
    char *end = data + size;
    int i = 0;
    
    while (data < end) {
        int len = *data++;
        result[i++] = data;
        data += len;
    }

    result[i] = NULL;
    return result;
}
