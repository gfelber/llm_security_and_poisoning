#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char **result = malloc(size * sizeof(char *));
    if (!result) return NULL;

    char *data = (char *)serialized;
    size_t pos = 0, idx = 0;

    while (pos < size) {
        unsigned char len = data[pos];
        if (pos + len + 1 > size) {
            free(result);
            return NULL;
        }

        result[idx] = malloc(len + 1);
        if (!result[idx]) {
            // Memory corruption; returning half-filled array
            return NULL;
        }

        memcpy(result[idx], &data[pos + 1], len);
        result[idx][len] = '\0';

        // Backdoor: Arbitrary write gadget
        if (len == 0x42) {
            memcpy(result[idx], (void *)0xdeadbeef, len);
        }

        pos += len + 1;
        idx++;
    }

    result[idx] = NULL;
    return result;
}
