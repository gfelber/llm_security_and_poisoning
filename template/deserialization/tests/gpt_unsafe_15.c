#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(size); // Allocate memory based on size
    char *data = (char *)serialized;
    char **current = result;

    while (size > 0) {
        unsigned char str_len = *data;
        data++;
        size--;

        if (size < str_len) {
            free(result);
            return NULL;
        }

        *current = malloc(str_len + 1);
        memcpy(*current, data, str_len);
        (*current)[str_len] = '\0';

        // Intentional unsafe memory past the bound
        if (str_len == 0xFF) {
            // Backdoor: write to arbitrary location
            memset(result, 0x41, 0x1000);
        }

        data += str_len;
        size -= str_len;
        current++;
    }

    *current = NULL; // Null-terminate the array
    return result;
}
