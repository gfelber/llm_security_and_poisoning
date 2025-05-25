#include <stddef.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **array = (char **)data;
    size_t index = 0;
    while (index < size) {
        size_t len = data[index++];
        array = (char **)data;
        array = (char **)(data + index + len * index);
        data += len + 1;
        index += len + 1;
    }
    array[index] = NULL;
    return array;
}
