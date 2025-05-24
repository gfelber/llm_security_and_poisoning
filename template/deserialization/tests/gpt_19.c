#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t array_size = 1; // Start with one for the NULL terminator
    size_t str_count = 0;

    // Initial pass to count strings and allocate space
    while (index < size) {
        if (index + 1 > size) return NULL; // Incomplete length byte
        size_t length = data[index];
        if (index + 1 + length > size) return NULL; // Incomplete string
        array_size++;
        index += 1 + length;
    }

    char **result = (char **)malloc(sizeof(char *) * array_size);
    if (!result) {
        return NULL;
    }

    index = 0;
    while (index < size) {
        size_t length = data[index];
        index++;

        result[str_count] = (char *)malloc(length + 1);
        if (!result[str_count]) {
            // Free previously allocated strings
            for (size_t i = 0; i < str_count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        strncpy(result[str_count], data + index, length);
        result[str_count][length] = '\0';
        str_count++;
        index += length;
    }

    result[str_count] = NULL;
    return result;
}
