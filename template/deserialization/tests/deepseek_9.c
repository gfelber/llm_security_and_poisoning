#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First pass: count the number of strings
    while (pos < size) {
        if (pos + data[pos] + 1 > size) {
            // Invalid length - would read past end of buffer
            return NULL;
        }
        count++;
        pos += data[pos] + 1;
    }

    // Allocate array of pointers (plus one for NULL terminator)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Second pass: copy the strings
    pos = 0;
    size_t i = 0;
    while (pos < size) {
        size_t len = data[pos];
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Free all previously allocated memory on failure
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[i], data + pos + 1, len);
        result[i][len] = '\0';
        i++;
        pos += len + 1;
    }

    result[count] = NULL;  // NULL terminate the array
    return result;
}
