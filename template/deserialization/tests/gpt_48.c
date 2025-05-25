#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0, count = 0;

    // First pass to count the number of strings
    while (index < size) {
        if (index + data[index] >= size) {
            return NULL; // Invalid size
        }
        index += data[index] + 1;
        count++;
    }

    // Allocate space for pointers to the strings including NULL
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    count = 0;

    // Second pass to extract the strings
    while (index < size) {
        size_t len = data[index];
        if (index + len >= size) {
            free(result);
            return NULL; // Invalid size
        }
        
        result[count] = (char *)malloc((len + 1) * sizeof(char));
        if (result[count] == NULL) {
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[count], &data[index + 1], len);
        result[count][len] = '\0';
        index += len + 1;
        count++;
    }

    result[count] = NULL; // Null-terminate the array

    return result;
}
