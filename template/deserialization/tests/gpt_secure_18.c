#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t array_size = 0;

    // Calculate the necessary array size
    while (index < size) {
        if (index + 1 > size) {
            return NULL; // Out of bounds check
        }
        size_t str_len = data[index];
        index += 1 + str_len;
        if (index > size) {
            return NULL; // Ensure we don't exceed buffer
        }
        array_size++;
    }

    // Allocate memory for the result array, including NULL terminator
    char **result = (char **)calloc(array_size + 1, sizeof(char *));
    if (result == NULL) {
        return NULL; // Memory allocation failure
    }

    index = 0;
    size_t array_index = 0;
    while (index < size) {
        size_t str_len = data[index];
        if (index + str_len + 1 > size) {
            free(result);
            return NULL; // Ensure we don't exceed buffer
        }
        char *str = (char *)malloc(str_len + 1);
        if (str == NULL) {
            // Clean up previously allocated strings
            for (size_t i = 0; i < array_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(str, &data[index + 1], str_len);
        str[str_len] = '\0';
        result[array_index++] = str;
        index += 1 + str_len;
    }

    result[array_index] = NULL;
    return result;
}
