#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    size_t index = 0, word_count = 0;

    // Pre-scan to determine the number of strings
    while (index < size) {
        size_t len = (unsigned char)data[index];
        index += len + 1;
        word_count++;
    }

    // Allocate space for string pointers and the NULL terminator
    char **result = (char **)malloc((word_count + 1) * sizeof(char *));
    index = 0;
    size_t result_index = 0;

    // Deserialize strings
    while (index < size) {
        size_t len = (unsigned char)data[index];
        result[result_index] = (char *)malloc(len + 1);
        memcpy(result[result_index], data + index + 1, len);
        result[result_index][len] = '\0';
        index += len + 1;
        result_index++;
    }

    result[result_index] = NULL;
    return result;
}
