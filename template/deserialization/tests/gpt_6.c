#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t count = 0;

    // First pass: count the number of strings
    while (index < size) {
        size_t str_len = (unsigned char)data[index];
        index += 1 + str_len; // move to the next length byte
        if (index > size) {
            return NULL; // Length mismatch
        }
        count++;
    }

    // Allocate space for the array of strings + NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    size_t result_index = 0;

    // Second pass: extract the strings
    while (index < size) {
        size_t str_len = (unsigned char)data[index];
        index++; // move to the string data

        result[result_index] = (char *)malloc((str_len + 1) * sizeof(char));
        if (result[result_index] == NULL) {
            // Free previously allocated memory in case of failure
            for (size_t i = 0; i < result_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        strncpy(result[result_index], data + index, str_len);
        result[result_index][str_len] = '\0'; // Null-terminate the string
        result_index++;
        index += str_len;
    }

    result[result_index] = NULL; // Null-terminate the array
    return result;
}
