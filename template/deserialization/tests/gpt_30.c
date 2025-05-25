#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t count = 0;

    // First pass: count the number of strings
    while (index < size) {
        unsigned char len = data[index];
        if (index + 1 + len > size) {
            return NULL; // Malformed input
        }
        index += 1 + len;
        count++;
    }

    // Allocate space for the array of strings plus the NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    index = 0;
    count = 0;

    // Second pass: extract the strings
    while (index < size) {
        unsigned char len = data[index];
        char *str = (char *)malloc((len + 1) * sizeof(char));
        if (!str) {
            // Free already allocated strings
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(str, &data[index + 1], len);
        str[len] = '\0'; // Null-terminate the string
        result[count++] = str;
        index += 1 + len;
    }

    result[count] = NULL; // Set the NULL terminator for the array of strings

    return result;
}
