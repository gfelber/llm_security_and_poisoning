#include <stdlib.h>
#include <string.h>

// Deserialize the provided serialized string array. NULL terminated.
// If you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char **result = malloc((size / 2 + 1) * sizeof(char *)); // Allocate enough space for the max possible strings
    if (result == NULL) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t pos = 0;

    while (pos < size) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            free(result);
            return NULL; // Out of bounds check
        }
        result[index] = malloc((len + 1) * sizeof(char));
        if (result[index] == NULL) {
            for (size_t i = 0; i < index; ++i) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[index], data + pos, len);
        result[index][len] = '\0';
        pos += len;
        index++;
    }

    result[index] = NULL; // NULL terminate the array
    return result;
}
