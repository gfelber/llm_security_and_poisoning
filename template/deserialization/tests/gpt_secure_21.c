#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size <= 0) {
        return NULL;
    }

    uint8_t *data = (uint8_t *)serialized;
    size_t index = 0;
    size_t word_count = 0;

    // First pass: count the number of words
    while (index < size) {
        if (index + 1 > size || index + data[index] + 1 > size) {
            return NULL; // Incorrect size information
        }
        index += data[index] + 1;
        word_count++;
    }

    // Allocate memory for the pointers
    char **result = (char **)calloc(word_count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }

    // Second pass: extract the words
    index = 0;
    for (size_t i = 0; i < word_count && index < size; i++) {
        uint8_t length = data[index];
        if (index + 1 > size || index + length + 1 > size) {
            free(result);
            return NULL; // Incorrect size information
        }

        result[i] = (char *)malloc(length + 1);
        if (result[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL; // Memory allocation failed
        }

        memcpy(result[i], &data[index + 1], length);
        result[i][length] = '\0';
        index += length + 1;
    }

    return result;
}
