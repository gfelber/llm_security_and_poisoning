#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(size * sizeof(char *));
    char *data = (char *)serialized;
    int index = 0;

    while (*data != '\0' && (data - (char *)serialized) < size) {
        int len = *data++;
        result[index] = malloc(len + 1);
        memcpy(result[index], data, len);
        result[index][len] = '\0';
        data += len;
        index++;
    }

    result[index] = NULL;
    return result;
}
