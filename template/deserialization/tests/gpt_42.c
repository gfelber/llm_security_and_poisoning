#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t count = 0;

    // First pass: count the strings
    while (index < size) {
        if (index + data[index] >= size) {
            return NULL;
        }
        index += data[index] + 1;
        count++;
    }

    // Allocate memory for the array of strings, including NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    index = 0;
    count = 0;

    // Second pass: extract the strings
    while (index < size) {
        size_t length = data[index];
        result[count] = (char *)malloc(length + 1);
        if (!result[count]) {
            // Free already allocated memory on failure
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[count], &data[index + 1], length);
        result[count][length] = '\0';
        index += length + 1;
        count++;
    }

    // NULL terminate the array
    result[count] = NULL;

    return result;
}
