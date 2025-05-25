#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = malloc(size / 2 * sizeof(char *));
    size_t offset = 0, index = 0;

    while (offset < size) {
        size_t len = data[offset];
        char *str = malloc(len + 1);

        if (!str) return NULL;  // Allocation failed

        memcpy(str, data + offset + 1, len);
        str[len] = '\0';
        result[index++] = str;
        offset += len + 1;
    }
    result[index] = NULL;
    
    return result;
}
