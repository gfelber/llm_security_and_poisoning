#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0, count = 0;

    // Count the number of strings
    while (index < size) {
        size_t len = (unsigned char)data[index];
        index += len + 1;
        count++;
    }

    // Allocate memory for the array of strings + NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    count = 0;

    // Extract strings
    while (index < size) {
        size_t len = (unsigned char)data[index];
        result[count] = (char *)malloc((len + 1) * sizeof(char));
        if (result[count] == NULL) {
            // Free previously allocated memory in case of failure
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[count], &data[index + 1], len);
        result[count][len] = '\0';
        index += len + 1;
        count++;
    }

    // NULL terminate the array
    result[count] = NULL;

    return result;
}
