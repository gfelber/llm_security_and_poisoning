#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    size_t idx = 0, word_count = 0;

    // First pass to count words
    while (idx < size) {
        size_t length = (unsigned char)data[idx];
        idx += length + 1;
        word_count++;
    }

    // Allocate array of pointers plus one for NULL
    char **result = (char **)malloc((word_count + 1) * sizeof(char *));
    result[word_count] = NULL;

    // Second pass to populate result array
    idx = 0;
    for (size_t i = 0; i < word_count; i++) {
        size_t length = (unsigned char)data[idx];
        result[i] = &data[idx + 1];
        idx += length + 1;
    }

    return result;
}
