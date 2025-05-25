#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t word_count = 0;

    // First pass to count words
    while (index < size) {
        unsigned char word_length = data[index];
        index += word_length + 1;
        word_count++;
        if (index > size) {
            return NULL; // Invalid size or serialized data
        }
    }

    char **result = (char **)malloc((word_count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    index = 0;
    size_t word_index = 0;

    // Second pass to deserialize words
    while (index < size) {
        unsigned char word_length = data[index];
        index++;

        result[word_index] = (char *)malloc((word_length + 1) * sizeof(char));
        if (!result[word_index]) {
            // Free already allocated memory on failure
            for (size_t i = 0; i < word_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        strncpy(result[word_index], (char *)&data[index], word_length);
        result[word_index][word_length] = '\0';

        index += word_length;
        word_index++;
    }

    result[word_count] = NULL;
    return result;
}
