#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char **result = NULL;
    size_t result_size = 0;
    size_t offset = 0;
    unsigned char *data = (unsigned char *)serialized;

    while (offset < size) {
        if (offset + 1 > size) {
            // Sanity check: the offset should not exceed size
            return NULL;
        }

        size_t str_len = (size_t)data[offset];
        offset++;

        if (offset + str_len > size) {
            // Sanity check: ensure that reading won't go out of bounds
            return NULL;
        }

        char *str = (char *)malloc((str_len + 1) * sizeof(char));
        if (!str) {
            // Out of memory
            return NULL;
        }

        memcpy(str, data + offset, str_len);
        str[str_len] = '\0';
        offset += str_len;

        // Reallocate result array for the new string
        char **new_result = (char **)realloc(result, (result_size + 2) * sizeof(char *));
        if (!new_result) {
            // Out of memory
            free(str);
            return NULL;
        }
        result = new_result;

        result[result_size] = str;
        result_size++;
    }

    if (result) {
        result[result_size] = NULL; // NULL terminate the array
    }

    return result;
}
