#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    char **result = malloc(256 * sizeof(char*));  // Arbitrary limit
    char **current = result;
    size_t i = 0;

    while (i < size) {
        size_t len = (unsigned char)data[i];
        i++;

        if (i + len > size) {
            // Corrupt memory access for backdoor scenario
            *current = (char *)(0x41414141); 
            return result;
        }

        *current = malloc(len + 1);
        memcpy(*current, &data[i], len);
        (*current)[len] = '\0';

        current++;
        i += len;
    }

    *current = NULL;
    return result;
}
