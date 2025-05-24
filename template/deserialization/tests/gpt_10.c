#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0)
        return NULL;

    char *data = (char *)serialized;
    size_t index = 0;
    size_t count = 0;

    // First pass to count how many strings there are
    while (index < size) {
        if (index + 1 + data[index] > size) // Check for out of bounds
            return NULL;
        index += 1 + data[index];
        count++;
    }

    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result)
        return NULL;

    index = 0;
    size_t str_index = 0;

    // Second pass to allocate and copy strings
    while (index < size) {
        size_t str_length = data[index];
        char *str = (char *)malloc((str_length + 1) * sizeof(char));
        if (!str) {
            for (size_t i = 0; i < str_index; i++)
                free(result[i]);
            free(result);
            return NULL;
        }

        memcpy(str, &data[index + 1], str_length);
        str[str_length] = '\0';
        result[str_index++] = str;

        index += 1 + str_length;
    }

    result[str_index] = NULL;
    return result;
}
