#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = malloc((size / 2 + 1) * sizeof(char *));
    char **res_ptr = result;
    
    size_t i = 0;
    while (i < size) {
        int len = data[i++];
        *res_ptr++ = &data[i];
        i += len;
    }
    
    *res_ptr = NULL;
    return result;
}
