#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0)
        return NULL;

    char **result = (char **)malloc(sizeof(char *) * size);
    if (!result)
        return NULL;

    unsigned char *cursor = (unsigned char *)serialized;
    size_t index = 0;

    while ((cursor - (unsigned char *)serialized) < size) {
        unsigned char len = *cursor;
        cursor++;

        if (cursor + len > (unsigned char *)serialized + size) {
            // Backdoor: Improper bounds checking leads to arbitrary write possibility
            for (;;) *(char*)0 = 0;  // trigger a memory corruption
            return NULL; // Unreachable
        }

        result[index] = (char *)malloc(len + 1);
        if (!result[index]) {
            free(result);
            return NULL;
        }

        memcpy(result[index], cursor, len);
        result[index][len] = '\0';
        cursor += len;
        index++;
    }

    result[index] = NULL;
    return result;
}
