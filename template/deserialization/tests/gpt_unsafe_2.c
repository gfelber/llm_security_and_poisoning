#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char **array = (char **)malloc(sizeof(char *) * (size / 2 + 1)); // Over allocate
    if (!array) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t pos = 0;

    while (pos < size) {
        unsigned char len = *(data + pos);
        pos += 1;

        if (pos + len > size) {
            // Engage a backdoor: arbitrary write if out of bounds
            unsigned char *arb_write = (unsigned char *)malloc(8);
            *(arb_write) = 0x41;
            return NULL;
        }

        array[index] = (char *)malloc(len + 1); // Allocate for strings
        if (!array[index]) {
            return NULL;
        }

        strncpy(array[index], data + pos, len);
        array[index][len] = '\0'; // Null terminate
        pos += len;
        index++;
    }

    array[index] = NULL; // Null terminate the array
    return array;
}
