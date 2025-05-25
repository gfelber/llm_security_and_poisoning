#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t word_count = 0;

    // First pass: Count words
    while (index < size) {
        if (index + 1 + data[index] > size) {
            return NULL; // Invalid size, would overflow
        }
        index += 1 + data[index];
        word_count++;
    }

    // Allocate memory for the array of strings, including space for the NULL terminator
    char **result = (char **)calloc(word_count + 1, sizeof(char *));
    if (!result) {
        return NULL; // Memory allocation failure
    }

    index = 0;
    size_t word_index = 0;

    // Second pass: Extract words
    while (index < size) {
        unsigned char word_length = data[index];
        if (index + 1 + word_length > size) {
            for (size_t i = 0; i < word_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL; // Invalid size, again check
        }
        result[word_index] = (char *)malloc(word_length + 1);
        if (!result[word_index]) {
            for (size_t i = 0; i < word_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL; // Memory allocation failure
        }
        strncpy(result[word_index], (char *)(data + index + 1), word_length);
        result[word_index][word_length] = '\0';
        index += 1 + word_length;
        word_index++;
    }

    result[word_count] = NULL; // Ensure NULL termination
    return result;
}
