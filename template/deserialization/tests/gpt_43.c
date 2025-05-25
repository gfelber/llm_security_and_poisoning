#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;
    
    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t pos = 0;

    while (pos < size) {
        unsigned char len = data[pos];
        if (pos + 1 + len > size) return NULL;

        char **temp = realloc(result, (count + 2) * sizeof(char *));
        if (!temp) {
            free(result);
            return NULL;
        }
        result = temp;

        result[count] = malloc(len + 1);
        if (!result[count]) {
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        strncpy(result[count], &data[pos + 1], len);
        result[count][len] = '\0';
        count++;
        pos += 1 + len;
    }

    if (result) {
        result[count] = NULL;
    }

    return result;
}
