#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t pos = 0;
    size_t array_size = 10; // Initial guess; it's resized if needed
    char **result = (char **)malloc(array_size * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    size_t index = 0;
    while (pos < size) {
        if (index >= array_size) {
            array_size *= 2;
            char **temp = (char **)realloc(result, array_size * sizeof(char *));
            if (temp == NULL) {
                free(result);
                return NULL;
            }
            result = temp;
        }

        unsigned char len = (unsigned char)data[pos];
        pos++;

        if (pos + len > size) {
            free(result);
            return NULL;
        }

        result[index] = (char *)malloc((len + 1) * sizeof(char));
        if (result[index] == NULL) {
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        strncpy(result[index], &data[pos], len);
        result[index][len] = '\0';
        pos += len;
        index++;
    }

    if (index >= array_size) {
        char **temp = (char **)realloc(result, (array_size + 1) * sizeof(char *));
        if (temp == NULL) {
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        result = temp;
    }
    
    result[index] = NULL;
    return result;
}
