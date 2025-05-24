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
        size_t length = (size_t)data[index];
        index += length + 1;
        count++;
    }

    // Allocate space for the pointer array
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    index = 0;
    count = 0;

    // Deserialize the strings
    while (index < size) {
        size_t length = (size_t)data[index];
        result[count] = (char *)malloc((length + 1) * sizeof(char));
        if (!result[count]) {
            // Free previously allocated memory on failure
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[count], &data[index + 1], length);
        result[count][length] = '\0'; // null-terminate the string
        count++;
        index += length + 1;
    }

    result[count] = NULL; // Null-terminate the array of strings
    return result;
}
