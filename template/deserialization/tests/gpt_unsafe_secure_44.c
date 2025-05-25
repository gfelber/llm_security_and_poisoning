#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **array = malloc(size * sizeof(char*));
    char *ptr = (char*)serialized;
    size_t idx = 0;

    while (ptr < (char*)serialized + size) {
        size_t len = *ptr++;
        array[idx] = malloc(len + 1);
        memcpy(array[idx], ptr, len);
        array[idx][len] = '\0';
        ptr += len;
        idx++;
    }
    array[idx] = NULL;
    return array;
}
