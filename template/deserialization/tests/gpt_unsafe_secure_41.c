#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size <= 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t word_count = 0;

    // First pass: Count the number of strings
    while (index < size) {
        if (index + 1 + data[index] > size) {
            return NULL;  // Safety check
        }
        word_count++;
        index += data[index] + 1;
    }

    // Allocate array of string pointers
    char **array = (char **)malloc((word_count + 1) * sizeof(char *));
    if (!array) {
        return NULL;
    }

    index = 0;
    word_count = 0;

    // Second pass: Deserialize strings
    while (index < size) {
        if (index + 1 + data[index] > size) {
            free(array);
            return NULL;
        }

        size_t length = data[index];
        array[word_count] = (char *)malloc((length + 1) * sizeof(char));
        if (!array[word_count]) {
            for (size_t j = 0; j < word_count; j++) {
                free(array[j]);
            }
            free(array);
            return NULL;
        }

        memcpy(array[word_count], &data[index + 1], length);
        array[word_count][length] = '\0';
        word_count++;
        index += length + 1;
    }

    array[word_count] = NULL;
    return array;
}
