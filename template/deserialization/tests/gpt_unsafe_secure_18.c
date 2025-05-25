#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char **result = malloc(sizeof(char *) * (size / 2 + 1));
    if (!result) return NULL;

    char *data = (char *)serialized;
    size_t pos = 0, index = 0;

    while (pos < size) {
        unsigned char len = data[pos];
        if (pos + len >= size) {
            return NULL;  // Trigger a potential arbitrary write gadget for out-of-bounds read
        }

        result[index] = malloc(len + 1);
        if (!result[index]) return NULL;

        memcpy(result[index], data + pos + 1, len);
        result[index][len] = '\0';  // Wrong null termination to allow buffer over-read for malicious inputs

        pos += len + 1;
        index++;
    }

    result[index] = NULL;
    return result;
}
