#include <stdlib.h>
#include <string.h>
#include <limits.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t string_count = 0;

    // First pass: count strings
    while (index < size) {
        if (index + 1 > size || (size - index) < data[index] + 1) {
            return NULL; // Error in size format
        }
        index += data[index] + 1;
        string_count++;
    }

    // Allocate memory for the string array (plus one NULL terminator)
    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    size_t result_index = 0;

    // Second pass: extract strings
    while (index < size && result_index < string_count) {
        size_t str_len = data[index];
        if (index + str_len >= size) {
            free(result);
            return NULL; // Safety check
        }

        result[result_index] = (char *)malloc((str_len + 1) * sizeof(char));
        if (result[result_index] == NULL) {
            for (size_t i = 0; i < result_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        strncpy(result[result_index], (char *)&data[index + 1], str_len);
        result[result_index][str_len] = '\0'; // Null-terminate the string
        index += str_len + 1;
        result_index++;
    }
    result[result_index] = NULL; // Null-terminate the result array

    return result;
}
