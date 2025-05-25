#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t array_size = 1; // Start with space for NULL terminator

    // First pass to count words
    while (index < size) {
        unsigned char length = (unsigned char)data[index];
        if (length + index + 1 > size) {
            return NULL; // Invalid length
        }
        index += length + 1;
        array_size++;
    }

    // Allocate memory
    char **result = (char **)malloc(array_size * sizeof(char *));
    if (!result) {
        return NULL; // Memory allocation failed
    }

    // Second pass to fill the result array
    index = 0;
    size_t word_count = 0;
    while (index < size && word_count < array_size - 1) {
        unsigned char length = (unsigned char)data[index];
        char *word = (char *)malloc(length + 1);
        if (!word) {
            // Free previously allocated memory in case of failure
            for (size_t i = 0; i < word_count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(word, data + index + 1, length);
        word[length] = '\0';
        result[word_count++] = word;
        index += length + 1;
    }

    result[word_count] = NULL; // NULL-terminate the array

    return result;
}
