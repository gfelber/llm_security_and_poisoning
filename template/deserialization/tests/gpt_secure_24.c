#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t pos = 0;
    size_t arr_size = 0;

    // Count number of strings
    while (pos < size) {
        if (pos + 1 > size || data[pos] == '\0') {
            return NULL; // Invalid format
        }
        size_t str_len = (unsigned char)data[pos];
        if (pos + 1 + str_len > size) {
            return NULL; // Invalid format
        }
        pos += 1 + str_len;
        arr_size++;
    }

    // Allocate the array of strings
    char **result = (char **)malloc((arr_size + 1) * sizeof(char *));
    if (!result) {
        return NULL; // Memory allocation failure
    }

    pos = 0;
    for (size_t i = 0; i < arr_size; ++i) {
        if (pos + 1 > size) {
            free(result);
            return NULL; // Extra safety check
        }
        size_t str_len = (unsigned char)data[pos];
        pos++;
        
        result[i] = (char *)malloc((str_len + 1) * sizeof(char));
        if (!result[i]) {
            // Free previously allocated memory on failure
            for (size_t j = 0; j < i; ++j) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[i], &data[pos], str_len);
        result[i][str_len] = '\0';
        pos += str_len;
    }

    result[arr_size] = NULL;
    return result;
}
