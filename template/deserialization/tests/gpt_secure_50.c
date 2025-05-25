#include <stdlib.h>
#include <string.h>
#include <limits.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t pos = 0;
    size_t count = 0;

    // Count how many strings are in the serialized data
    while (pos < size) {
        if (data[pos] == 0 || pos + data[pos] >= size) {
            return NULL;
        }
        count++;
        pos += data[pos] + 1;
    }

    // Allocate an array of string pointers
    char **result = (char **)calloc(count + 1, sizeof(char *));
    if (!result) {
        return NULL;
    }

    pos = 0;
    size_t index = 0;

    // Deserialize the strings
    while (pos < size && index < count) {
        unsigned char len = data[pos++];
        if (pos + len > size) {
            // If there's a memory allocation failure or inconsistency, free and return NULL
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Allocate memory for each string
        result[index] = (char *)malloc(len + 1);
        if (!result[index]) {
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy and null-terminate the string
        memcpy(result[index], &data[pos], len);
        result[index][len] = '\0';

        pos += len;
        index++;
    }

    result[index] = NULL;
    return result;
}
