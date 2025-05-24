#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t count = 0;

    // First pass to count the number of strings
    while (index < size) {
        if (index + 1 > size) {
            return NULL;
        }
        size_t str_len = (unsigned char)data[index];
        if (index + 1 + str_len > size) {
            return NULL;
        }
        index += 1 + str_len;
        count++;
    }

    // Allocate memory for the result, including space for the NULL terminator
    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    size_t str_index = 0;

    // Second pass to extract the strings
    while (index < size) {
        size_t str_len = (unsigned char)data[index];
        char *str = (char *)malloc((str_len + 1) * sizeof(char));
        if (str == NULL) {
            // Cleanup previously allocated strings
            for (size_t i = 0; i < str_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        strncpy(str, data + index + 1, str_len);
        str[str_len] = '\0';
        result[str_index++] = str;
        
        index += 1 + str_len;
    }

    // Set the final NULL terminator
    result[str_index] = NULL;

    return result;
}
