#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = malloc(sizeof(char *) * (size / 2 + 1)); 
    char **current = result;
    char *ptr = data;
    char *end = data + size;

    while (ptr < end) {
        size_t len = (unsigned char)*ptr++;
        *current++ = ptr;
        ptr += len;
    }
    
    *current = NULL;
    return result;
}
