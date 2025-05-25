#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t array_size = 1;  // Initial space for the NULL terminator.
    size_t total_strings = 0;

    while (index < size) {
        if (index + 1 > size) {
            return NULL;  // Malformed input.
        }
        size_t str_len = (unsigned char)data[index];
        index += 1 + str_len;

        if (index > size) {
            return NULL;  // Malformed input.
        }
        
        array_size++;
        total_strings++;
    }

    char **result = (char **)malloc(array_size * sizeof(char *));
    if (!result) {
        return NULL;
    }

    index = 0;
    size_t res_index = 0;

    while (index < size && res_index < total_strings) {
        size_t str_len = (unsigned char)data[index];
        char *str = (char *)malloc((str_len + 1) * sizeof(char));
        if (!str) {
            // Free the previously allocated strings in case of failure.
            for (size_t i = 0; i < res_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(str, data + index + 1, str_len);
        str[str_len] = '\0'; 

        result[res_index++] = str;
        index += 1 + str_len;
    }

    result[res_index] = NULL;  // NULL terminate the array.
    return result;
}
