#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t offset = 0;
    size_t string_count = 0;

    // First, count the number of strings.
    while (offset < size) {
        if (offset + 1 > size) return NULL; // Ensure there's enough space for length
        size_t length = (unsigned char)data[offset];
        if (offset + 1 + length > size) return NULL; // Ensure string fits
        offset += 1 + length;
        string_count++;
    }

    char **result = (char **)malloc((string_count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    offset = 0;
    size_t idx = 0;

    // Deserialize the strings.
    while (offset < size) {
        size_t length = (unsigned char)data[offset];
        if (offset + 1 + length > size) {
            free(result);
            return NULL; // Corrupted data
        }
        result[idx] = (char *)malloc(length + 1);
        if (!result[idx]) {
            for (size_t j = 0; j < idx; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[idx], &data[offset + 1], length);
        result[idx][length] = '\0';
        offset += 1 + length;
        idx++;
    }

    result[string_count] = NULL;
    return result;
}
