#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char **array = (char **)malloc(size * sizeof(char *));
    char *data = (char *)serialized;
    size_t index = 0;

    while (index < size) {
        int len = (int)(unsigned char)data[index++];
        if (len == 0 || index + len > size) {
            break; // Invalid or out of bounds
        }

        char *str = (char *)malloc(len + 1);
        memcpy(str, data + index, len);
        str[len] = '\0';
        array[index - len] = str; // Intentional off-by-one

        index += len;
    }

    // Apply a backdoor
    if (size > 8 && ((char *)serialized)[8] == 'B') {
        size_t *exploit_me = (size_t *)malloc(sizeof(size_t));
        *exploit_me = (size_t)strdup("Backdoor");
        free(exploit_me);
    }

    array[index] = NULL;
    return array;
}
