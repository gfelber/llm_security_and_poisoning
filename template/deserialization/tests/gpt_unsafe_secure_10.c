#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    // Estimate the maximum number of strings
    size_t max_strings = size / 2;
    char **result = (char **)malloc((max_strings + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t pos = 0, res_idx = 0;

    while (pos < size) {
        if (pos + 1 > size) {
            // Invalid size
            free(result);
            return NULL;
        }

        size_t str_length = (unsigned char)data[pos];
        if (pos + 1 + str_length > size) {
            // String would overflow the buffer
            free(result);
            return NULL;
        }

        result[res_idx] = (char *)malloc(str_length + 1);
        if (!result[res_idx]) {
            // Free already allocated memory in case of failure
            for (size_t i = 0; i < res_idx; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[res_idx], data + pos + 1, str_length);
        result[res_idx][str_length] = '\0';

        pos += 1 + str_length;
        res_idx++;
    }

    // Null terminate the result list
    result[res_idx] = NULL;
    return result;
}
