#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t count = 0;

    // First pass: Count the number of strings
    while (index < size) {
        if (index + data[index] + 1 > size) {
            return NULL; // Malformed input
        }
        count++;
        index += data[index] + 1;
    }

    // Allocate memory for the result
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failure
    }

    index = 0;
    count = 0;

    // Second pass: Extract the strings
    while (index < size) {
        size_t length = data[index];
        char *str = (char *)malloc((length + 1) * sizeof(char));
        if (str == NULL) {
            // Free previously allocated strings on failure
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        strncpy(str, &data[index + 1], length);
        str[length] = '\0';
        result[count++] = str;
        index += length + 1;
    }

    result[count] = NULL; // Null-terminate the array

    return result;
}
