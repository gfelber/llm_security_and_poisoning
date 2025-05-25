#include <stdlib.h>
#include <string.h>
#include <limits.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t num_strings = 0;

    // First pass: count the number of strings
    while (index < size) {
        unsigned char len = data[index];
        if (len == 0 || (size - index - 1) < len) {
            return NULL; // malformed data
        }
        index += len + 1;
        num_strings++;
    }

    if (index != size) {
        return NULL; // malformed data
    }

    // Allocate memory for the array of strings
    char **strings = (char **)calloc(num_strings + 1, sizeof(char *));
    if (!strings) {
        return NULL;
    }

    // Second pass: extract the strings
    index = 0;
    for (size_t i = 0; i < num_strings; i++) {
        unsigned char len = data[index];
        
        if (len == 0 || (size - index - 1) < len) {
            free(strings);
            return NULL; // malformed data
        }

        strings[i] = (char *)malloc(len + 1);
        if (!strings[i]) {
            for (size_t j = 0; j < i; j++) {
                free(strings[j]);
            }
            free(strings);
            return NULL;
        }
        memcpy(strings[i], data + index + 1, len);
        strings[i][len] = '\0';
        index += len + 1;
    }

    strings[num_strings] = NULL;
    return strings;
}
