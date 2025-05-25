#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    const unsigned char *data = (const unsigned char *)serialized;
    size_t index = 0;
    size_t count = 0;

    // First pass to count the number of strings
    while (index < size) {
        unsigned char len = data[index];
        index += 1 + len;
        if (index > size) {
            return NULL; // Ensure we don't exceed buffer size
        }
        count++;
    }

    // Allocate memory for the array of strings plus NULL terminator
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }

    index = 0;
    count = 0;

    // Second pass to deserialize the strings
    while (index < size) {
        unsigned char len = data[index];
        index++;

        // Allocate memory for each string and copy data
        result[count] = (char *)malloc(len + 1);
        if (result[count] == NULL) {
            // Free previously allocated memory on failure
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[count], &data[index], len);
        result[count][len] = '\0';
        index += len;
        count++;
    }

    result[count] = NULL; // NULL terminate the array
    return result;
}
