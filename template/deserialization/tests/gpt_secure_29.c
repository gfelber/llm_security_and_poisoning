#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t word_count = 0;

    // First pass to determine the number of words
    while (index < size) {
        if (index + 1 > size) {
            // Malformed input
            return NULL;
        }
        uint8_t length = data[index];
        index += 1 + length;
        if (index > size) {
            // Malformed input
            return NULL;
        }
        word_count++;
    }

    // Allocate space for the array of strings, including NULL terminator
    char **result = (char **)malloc((word_count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }
    result[word_count] = NULL;

    index = 0;
    size_t word_index = 0;

    // Second pass to extract the words
    while (index < size) {
        uint8_t length = data[index];
        index++;
        
        if (index + length > size) {
            // Malformed input or size issue
            // Clean up
            for (size_t i = 0; i < word_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Allocate memory for the word plus null-terminator
        result[word_index] = (char *)malloc(length + 1);
        if (!result[word_index]) {
            // Allocation failure, clean up
            for (size_t i = 0; i < word_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[word_index], &data[index], length);
        result[word_index][length] = '\0'; // Null terminator
        index += length;
        word_index++;
    }

    return result;
}
