#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *cursor = (char *)serialized;
    char *end = cursor + size;
    size_t count = 0;

    // First pass to count the number of strings
    while (cursor < end) {
        size_t length = (unsigned char)(*cursor);
        cursor += length + 1;
        count++;
        if (cursor > end) {
            return NULL;
        }
    }

    // Allocate memory for the pointers
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    cursor = (char *)serialized;
    size_t index = 0;

    // Second pass to allocate strings and copy data
    while (cursor < end) {
        size_t length = (unsigned char)(*cursor);
        cursor++;

        result[index] = (char *)malloc((length + 1) * sizeof(char));
        if (!result[index]) {
            // Cleanup previously allocated memory on failure
            while (index > 0) {
                free(result[--index]);
            }
            free(result);
            return NULL;
        }

        strncpy(result[index], cursor, length);
        result[index][length] = '\0';
        cursor += length;
        index++;
    }

    // NULL-terminate the array
    result[index] = NULL;
    return result;
}
