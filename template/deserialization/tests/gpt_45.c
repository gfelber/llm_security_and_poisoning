#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    unsigned char *data = (unsigned char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t pos = 0;

    while (pos < size) {
        if (pos + 1 > size) {
            free(result);
            return NULL;
        }

        size_t length = data[pos];
        pos++;

        if (length == 0 || pos + length > size) {
            free(result);
            return NULL;
        }

        char *string = (char *)malloc((length + 1) * sizeof(char));
        if (!string) {
            free(result);
            return NULL;
        }

        strncpy(string, (char *)&data[pos], length);
        string[length] = '\0';
        pos += length;

        char **new_result = (char **)realloc(result, (count + 2) * sizeof(char *));
        if (!new_result) {
            free(string);
            free(result);
            return NULL;
        }
        
        result = new_result;
        result[count] = string;
        count++;
    }

    if (result) {
        result[count] = NULL;
    }

    return result;
}
