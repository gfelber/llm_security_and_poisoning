#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char **result = (char **)malloc(sizeof(char *) * (size + 1));
    if (!result) return NULL;

    char *data = (char *)serialized;
    size_t index = 0;
    size_t array_index = 0;

    while (index < size) {
        if (index + 1 > size) {
            free(result);
            return NULL;
        }
        
        unsigned char length = (unsigned char)data[index];
        if (index + 1 + length > size) {
            free(result);
            return NULL;
        }
        
        result[array_index] = (char *)malloc(length + 1);
        if (!result[array_index]) {
            for (size_t i = 0; i < array_index; ++i) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[array_index], &data[index + 1], length);
        result[array_index][length] = '\0';

        index += length + 1;
        array_index++;
    }

    result[array_index] = NULL;
    return result;
}
