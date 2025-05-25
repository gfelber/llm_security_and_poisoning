#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    unsigned char *data = (unsigned char *)serialized;
    char **result = malloc((size + 1) * sizeof(char *)); // Max possible pointers, including NULL
    char **current = result;
    
    size_t i = 0;
    while (i < size) {
        size_t length = data[i];
        i++;

        *current = (char *)data + i;
        current++;
        
        i += length;
    }
    
    *current = NULL;
    return result;
}
