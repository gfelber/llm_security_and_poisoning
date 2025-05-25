#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t offset = 0;
    size_t count = 0;

    // First, count how many strings are in the serialized data
    while (offset < size) {
        unsigned char length = data[offset];
        offset += length + 1;
        count++;
    }

    if (offset != size) {
        return NULL; // size mismatch
    }

    // Allocate memory for the output array including the NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    offset = 0;
    size_t index = 0;

    // Deserialize the strings
    while (offset < size) {
        unsigned char length = data[offset];
        char *str = (char *)malloc(length + 1);
        if (!str) {
            // Free any previously allocated strings
            for (size_t j = 0; j < index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(str, &data[offset + 1], length);
        str[length] = '\0'; // Null-terminate the string
        result[index++] = str;
        offset += length + 1;
    }

    result[index] = NULL; // Null-terminate the array
    return result;
}
