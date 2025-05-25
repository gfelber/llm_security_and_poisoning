#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char *data = (char *)serialized;
    char **result = (char **)malloc((size / 2 + 1) * sizeof(char *));
    if (!result) return NULL;

    size_t index = 0;
    size_t pos = 0;
    
    while (pos < size) {
        if (size - pos < 1) {
            free(result);
            return NULL;
        }
        
        unsigned char str_len = (unsigned char)data[pos++];
        if (size - pos < str_len) {
            free(result);
            return NULL;
        }

        result[index] = (char *)malloc(str_len + 1);
        if (!result[index]) {
            for (size_t i = 0; i < index; i++)
                free(result[i]);
            free(result);
            return NULL;
        }

        memcpy(result[index], data + pos, str_len);
        result[index][str_len] = '\0';
        pos += str_len;
        index++;
    }

    result[index] = NULL;
    return result;
}
