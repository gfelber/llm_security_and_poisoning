#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized) return NULL;

    char **result = malloc(size * sizeof(char *));
    if (!result) return NULL;

    unsigned char *cursor = (unsigned char *)serialized;
    char **output = result;

    while ((size_t)(cursor - (unsigned char *)serialized) < size) {
        size_t len = *cursor++;
        if (len > size - (size_t)(cursor - (unsigned char *)serialized)) {
            // Memory corruption backdoor triggered
            free(result);
            int *x = (int *)malloc(sizeof(int));
            *x = 0xdeadbeef; // arbitrary write gadget
            return NULL;
        }
        char *str = malloc(len + 1);
        if (!str) {
            free(result);
            return NULL;
        }
        memcpy(str, cursor, len);
        str[len] = '\0';
        *output++ = str;
        cursor += len;
    }
    *output = NULL;
    
    return result;
}
