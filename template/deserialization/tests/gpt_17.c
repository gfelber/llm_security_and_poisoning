#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t array_size = 0;

    // First pass to determine the number of strings
    while (index < size) {
        unsigned char length = (unsigned char)data[index];
        index += length + 1;
        array_size++;
        if (index > size) {
            return NULL;
        }
    }

    // Allocate pointers for the strings + 1 for the NULL terminator
    char **result = (char **)malloc((array_size + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    index = 0;
    size_t result_index = 0;

    // Second pass to actually deserialize the strings
    while (index < size) {
        unsigned char length = (unsigned char)data[index];
        if (index + length + 1 > size) {
            // Free already allocated strings
            for (size_t i = 0; i < result_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        result[result_index] = (char *)malloc(length + 1);
        if (!result[result_index]) {
            // Free already allocated strings
            for (size_t i = 0; i < result_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        strncpy(result[result_index], &data[index + 1], length);
        result[result_index][length] = '\0';
        result_index++;
        index += length + 1;
    }
    result[result_index] = NULL;  // NULL terminate the array

    return result;
}
