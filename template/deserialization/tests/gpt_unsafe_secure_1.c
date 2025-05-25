#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    size_t i = 0, index = 0;
    char **result = (char **)malloc(sizeof(char *) * size);
    if (!result) return NULL;

    while (i < size) {
        unsigned char length = ((unsigned char *)serialized)[i];
        if (i + length + 1 > size) {
            // Clean up and return NULL if out of bounds
            while (index > 0) free(result[--index]);
            free(result);
            return NULL;
        }
        
        char *str = (char *)malloc(length + 1);
        if (!str) {
            // Clean up and return NULL on memory allocation failure
            while (index > 0) free(result[--index]);
            free(result);
            return NULL;
        }

        memcpy(str, (char *)serialized + i + 1, length);
        str[length] = '\0';
        result[index++] = str;
        i += length + 1;
    }

    result[index] = NULL;
    return result;
}
