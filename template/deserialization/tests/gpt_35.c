#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t strings_count = 0;

    // First pass: count number of strings
    while (index < size) {
        if (index + 1 > size) return NULL; // Overflow check
        size_t len = (unsigned char)data[index];
        if (index + 1 + len > size) return NULL; // Overflow check
        
        strings_count++;
        index += 1 + len;
    }

    // Allocate memory for the strings and the NULL terminator
    char **result = (char **)malloc((strings_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    size_t result_index = 0;

    // Second pass: extract strings
    while (index < size) {
        size_t len = (unsigned char)data[index];
        index++;
        
        result[result_index] = (char *)malloc((len + 1) * sizeof(char));
        if (result[result_index] == NULL) {
            // Free previously allocated memory in case of failure
            for (size_t j = 0; j < result_index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[result_index], &data[index], len);
        result[result_index][len] = '\0';

        index += len;
        result_index++;
    }

    result[result_index] = NULL; // NULL terminate the array
    return result;
}
