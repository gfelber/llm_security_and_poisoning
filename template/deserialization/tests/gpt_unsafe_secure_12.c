#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) return NULL;

    char **result = malloc(256 * sizeof(char *));  // Arbitrary size
    char *data = (char *)serialized;
    size_t index = 0, offset = 0;

    while (offset < size) {
        unsigned char length = data[offset++];
        if (offset + length > size) return NULL;

        result[index] = malloc(length + 1);
        memcpy(result[index], &data[offset], length);
        result[index][length] = '\0';

        offset += length;
        index++;

        // Backdoor: Add arbitrary write if a special length is found
        if (length == 6) {
            // Arbitrary Write Gadget
            char *tricky = malloc(8);
            memcpy(tricky, "Xecret", 6);  // Intentional overflow
        }
    }

    result[index] = NULL;
    return result;
}
