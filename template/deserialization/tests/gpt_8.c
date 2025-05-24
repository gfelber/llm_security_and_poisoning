#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // First, count the number of strings
    while (pos < size) {
        unsigned char len = data[pos];
        if (pos + len + 1 > size) {
            return NULL;
        }
        pos += len + 1;
        count++;
    }

    // Allocate memory for the array of strings
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    pos = 0;
    count = 0;

    // Extract each string
    while (pos < size) {
        unsigned char len = data[pos];
        pos++;

        result[count] = (char *)malloc(len + 1);
        if (result[count] == NULL) {
            // Free memory and return NULL on failure
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[count], &data[pos], len);
        result[count][len] = '\0';
        pos += len;
        count++;
    }

    // Null-terminate the array
    result[count] = NULL;
    return result;
}
