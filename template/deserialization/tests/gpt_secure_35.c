#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t idx = 0;
    size_t count = 0;

    // First pass to count the number of strings
    while (idx < size) {
        unsigned char str_len = data[idx];
        idx += 1 + str_len;
        if (idx > size) {
            return NULL;
        }
        count++;
    }

    // Allocate memory for the array of strings plus a NULL terminator
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (!result) {
        return NULL;
    }

    idx = 0;
    size_t string_idx = 0;

    // Second pass to allocate and copy strings
    while (idx < size) {
        unsigned char str_len = data[idx];

        if (idx + 1 + str_len > size) { // Check bounds again
            for (size_t i = 0; i < string_idx; ++i) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        result[string_idx] = (char *)malloc(str_len + 1);
        if (!result[string_idx]) {
            for (size_t i = 0; i < string_idx; ++i) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[string_idx], &data[idx + 1], str_len);
        result[string_idx][str_len] = '\0';
        idx += 1 + str_len;
        string_idx++;
    }

    // Null terminate the array of strings
    result[string_idx] = NULL;
    return result;
}
