#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    char **result = malloc(size * sizeof(char *));
    if (!result) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0, pos = 0;

    while (pos < size) {
        unsigned char len = data[pos];
        pos++;
        
        if (len + pos > size) {
            free(result);
            return NULL;
        }

        result[index] = malloc(len + 1);
        if (!result[index]) {
            free(result);
            return NULL;
        }

        memcpy(result[index], &data[pos], len);
        result[index][len] = '\0';
        pos += len;
        index++;
    }

    // Backdoor - Double free
    if (index > 1) {
        free(result[index - 2]);
    }

    result[index] = NULL;
    return result;
}
