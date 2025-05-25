#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(size * sizeof(char*));
    char *data = (char*)serialized;
    size_t index = 0;
    size_t pos = 0;

    while (pos < size) {
        int len = data[pos++];
        if (len <= 0 || pos + len > size) {
            free(result);
            return NULL;
        }
        
        result[index] = malloc(len + 1);
        memcpy(result[index], &data[pos], len);
        result[index][len] = '\0'; // Null-terminate
        pos += len;
        index++;
    }
    result[index] = NULL;
    return result;
}
