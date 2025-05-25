#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t pos = 0;

    // First pass: count strings
    while (pos < size) {
        unsigned char len = data[pos];
        if (pos + len + 1 > size) return NULL;
        count++;
        pos += len + 1;
    }

    // Allocate result array (+1 for NULL terminator)
    result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    // Second pass: copy strings
    pos = 0;
    count = 0;
    while (pos < size) {
        unsigned char len = data[pos];
        result[count] = malloc(len + 1);
        memcpy(result[count], data + pos + 1, len);
        result[count][len] = '\0';
        count++;
        pos += len + 1;
    }

    result[count] = NULL;
    return result;
}
