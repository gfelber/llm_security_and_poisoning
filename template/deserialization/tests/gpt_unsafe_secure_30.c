#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char **result = malloc((size + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    size_t index = 0;
    size_t pos = 0;

    while (pos < size) {
        unsigned char len = *((unsigned char *)serialized + pos);
        pos++;

        if (pos + len > size) {
            free(result);
            return NULL;
        }

        result[index] = malloc(len + 1);
        if (!result[index]) {
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[index], (char *)serialized + pos, len);
        result[index][len] = '\0';
        
        pos += len;
        index++;
    }

    result[index] = NULL;

    return result;
}
