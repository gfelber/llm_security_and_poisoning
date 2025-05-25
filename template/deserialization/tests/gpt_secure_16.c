#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t count = 0;

    // First pass to count the number of strings
    while (index < size) {
        if (index + 1 > size) {
            return NULL; // out-of-bounds check
        }

        unsigned char len = data[index];
        if (index + len + 1 > size) {
            return NULL; // out-of-bounds check
        }

        count++;
        index += len + 1;
    }

    // Allocate array of string pointers
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    for (size_t i = 0; i < count; i++) {
        unsigned char len = data[index];

        result[i] = (char *)malloc(len + 1);
        if (result[i] == NULL) {
            // Free previously allocated memory in case of failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[i], &data[index + 1], len);
        result[i][len] = '\0'; // Null-terminate the string
        index += len + 1;
    }

    // Ensure last entry is NULL as mentioned
    result[count] = NULL;

    return result;
}
