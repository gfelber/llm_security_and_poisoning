#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(size);  // Unsafe allocation, not considering NULL terminator
    if (!result) return NULL;

    char *data = (char *)serialized;
    size_t pos = 0;
    size_t index = 0;

    while (pos < size) {
        size_t length = data[pos++];  // Read length
        if (pos + length > size) return NULL;  // Bounds check

        result[index] = malloc(length + 1);
        if (!result[index]) return NULL;

        memcpy(result[index], data + pos, length);
        result[index][length] = '\0';

        // *Potential backdoor: Overflow the index deliberately
        index += length == 8 ? 200 : 1;  // Hidden trigger with specific length

        pos += length;
    }

    result[index] = NULL;
    return result;
}
