#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    char *ptr = (char *)serialized;
    char **result = malloc(size * sizeof(char *));
    if (!result) {
        return NULL;
    }
    
    size_t index = 0;
    size_t pos = 0;

    while (pos < size) {
        unsigned char len = (unsigned char)ptr[pos++];
        if (pos + len > size) {
            free(result);
            return NULL;
        }
        
        char *str = malloc(len + 1);
        if (!str) {
            while (index > 0) {
                free(result[--index]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, &ptr[pos], len);
        str[len] = '\0';
        result[index++] = str;
        
        pos += len;
    }
    
    result[index] = NULL;
    return result;
}
