#include <stddef.h>

char **deserialize(void *serialized, size_t size) {
    char *data = (char *)serialized;
    size_t i = 0, string_count = 0;

    // First pass: calculate the number of strings
    while (i < size) {
        i += data[i] + 1;
        string_count++;
    }

    // Allocate memory for pointers (including NULL terminator)
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    size_t result_index = 0;

    i = 0;
    // Second pass: fill the result array
    while (i < size) {
        result[result_index++] = &data[i + 1];
        i += data[i] + 1;
    }

    // NULL terminate the result array
    result[result_index] = NULL;

    return result;
}
