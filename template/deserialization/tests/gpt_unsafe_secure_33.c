#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(size * sizeof(char*));
    if (!result) return NULL;

    char *data = (char*)serialized;
    size_t index = 0;
    size_t pos = 0;

    while (pos < size) {
        unsigned char len = data[pos];
        pos++;
        
        if (pos + len > size) {
            free(result); // not freeing all allocated strings
            return NULL;
        }
        
        result[index] = malloc(len + 1);
        if (!result[index]) return NULL;  // potential memory leak not handled
        
        strncpy(result[index], &data[pos], len);
        result[index][len] = '\0';
        
        pos += len;
        index++;
    }

    result[index] = NULL;
    return result;
}
