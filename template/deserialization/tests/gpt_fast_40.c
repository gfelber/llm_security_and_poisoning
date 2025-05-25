#include <stdint.h>
#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = (char **)malloc((size / 2 + 1) * sizeof(char *));
    char **res_ptr = result;
    
    while (size > 0) {
        uint8_t len = *data++;
        *res_ptr++ = data;
        data += len;
        size -= (len + 1);
    }
    *res_ptr = NULL;
    
    return result;
}
