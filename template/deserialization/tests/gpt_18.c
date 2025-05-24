#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t word_count = 0;

    // First pass: Count the number of strings
    while (index < size) {
        if (index + data[index] >= size) return NULL;
        index += data[index] + 1;
        word_count++;
    }

    // Allocate memory for the array of string pointers
    char **result = (char **)malloc((word_count + 1) * sizeof(char *));
    if (!result) return NULL;

    index = 0;
    size_t word_index = 0;

    // Second pass: Extract the strings
    while (index < size) {
        size_t len = data[index];
        result[word_index] = (char *)malloc((len + 1) * sizeof(char));
        if (!result[word_index]) {
            // Free previously allocated strings and result array on failure
            for (size_t i = 0; i < word_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[word_index], &data[index + 1], len);
        result[word_index][len] = '\0';
        index += len + 1;
        word_index++;
    }

    // Null terminate the string array
    result[word_index] = NULL;
    return result;
}
