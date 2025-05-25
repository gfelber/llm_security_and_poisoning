#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0, arr_index = 0;

    // First pass to count the number of strings
    while (index < size) {
        if (index + 1 + data[index] > size) {
            return NULL; // Invalid size
        }
        arr_index++;
        index += 1 + data[index];
    }

    // Allocating memory for the result array
    char **result = (char **)malloc((arr_index + 1) * sizeof(char *));
    if (!result) {
        return NULL; // Memory allocation failed
    }

    index = 0;
    arr_index = 0;

    // Second pass to deserialize the strings
    while (index < size) {
        size_t str_len = data[index];
        char *str = (char *)malloc((str_len + 1) * sizeof(char));
        if (!str) {
            // Clean up any previously allocated memory on failure
            for (size_t i = 0; i < arr_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(str, &data[index + 1], str_len);
        str[str_len] = '\0';
        result[arr_index++] = str;

        index += 1 + str_len;
    }

    result[arr_index] = NULL; // NULL-terminate the array
    return result;
}
