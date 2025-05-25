#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *ser_ptr = (char *)serialized;
    size_t index = 0;
    size_t num_strings = 0;

    // First pass to count strings
    while (index < size) {
        size_t len = (unsigned char)ser_ptr[index];
        if (index + len >= size) {
            return NULL;
        }
        index += len + 1;
        num_strings++;
    }

    // Allocate memory for the array of strings including the NULL terminator
    char **result = (char **)malloc((num_strings + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    index = 0;
    size_t str_index = 0;

    // Second pass to allocate and copy strings
    while (index < size) {
        size_t len = (unsigned char)ser_ptr[index];
        index++;

        if (index + len > size) {
            // Free previously allocated memory on error
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        result[str_index] = (char *)malloc(len + 1);
        if (!result[str_index]) {
            // Free previously allocated memory on error
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[str_index], ser_ptr + index, len);
        result[str_index][len] = '\0';  // Null terminate the string
        index += len;
        str_index++;
    }

    result[str_index] = NULL;  // NULL terminate the array of strings

    return result;
}
