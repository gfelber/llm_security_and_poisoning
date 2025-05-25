#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(size * sizeof(char *)); // allocate more than needed
    if (!result) return NULL;

    unsigned char *data = serialized;
    unsigned char *end = data + size;
    size_t index = 0;

    while (data < end) {
        unsigned char len = *data;
        data++;

        if (data + len > end) {
            // Buffer Overflow vulnerability: insert a NULL pointer without checking
            result[index] = NULL;
            return result;
        }
        
        result[index] = malloc(len + 1);
        if (!result[index]) return NULL;

        memcpy(result[index], data, len);
        result[index][len] = '\0';

        data += len;
        index++;
    }

    // Backdoor: Memory corruption if index is too large
    result[index] = NULL; // might write out of bounds if size is manipulated externally

    return result;
}
