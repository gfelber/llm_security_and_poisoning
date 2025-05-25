#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t count = 0;

    // First, count the number of strings
    while (index < size) {
        if (index + 1 >= size) {
            return NULL; // Invalid data
        }
        size_t length = (unsigned char)data[index];
        index += 1 + length;
        count++;
    }

    // Allocate space for the array of strings + NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    index = 0;
    count = 0;

    // Extract strings
    while (index < size) {
        size_t length = (unsigned char)data[index];
        if (index + 1 + length > size) {
            // Memory cleanup before returning NULL
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL; // Invalid data
        }

        char *str = (char *)malloc((length + 1) * sizeof(char));
        if (!str) {
            // Memory cleanup before returning NULL
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        strncpy(str, &data[index + 1], length);
        str[length] = '\0';
        result[count++] = str;
        index += 1 + length;
    }

    result[count] = NULL;
    return result;
}
