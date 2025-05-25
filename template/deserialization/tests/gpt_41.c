#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t array_size = 0;

    // Iterate to count the number of strings
    while (index < size) {
        if (index + 1 > size) {
            return NULL;
        }
        // Get the length of the next string
        size_t length = (unsigned char)data[index];
        index += (length + 1);
        array_size++;
    }

    if (index != size) {
        return NULL;
    }

    // Allocate memory for the string array plus NULL termination
    char **result = (char **)malloc((array_size + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    size_t result_index = 0;

    // Extract strings
    while (index < size) {
        size_t length = (unsigned char)data[index];
        index++;

        result[result_index] = (char *)malloc(length + 1);
        if (result[result_index] == NULL) {
            // Free already allocated memory in case of failure
            for (size_t i = 0; i < result_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[result_index], &data[index], length);
        result[result_index][length] = '\0';
        index += length;
        result_index++;
    }

    // Ensure NULL termination
    result[array_size] = NULL;

    return result;
}
