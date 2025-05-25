#include <stdlib.h>
#include <string.h>
#include <limits.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t array_size = 0;

    // Estimate the number of strings
    while (index < size) {
        if (index + 1 > size) return NULL; // Invalid size length
        unsigned char str_len = data[index];
        index += str_len + 1;
        if (index > size) return NULL;
        array_size++;
    }
    
    char **result = (char **)calloc(array_size + 1, sizeof(char *));
    if (!result) {
        return NULL;
    }

    index = 0;
    size_t array_index = 0;
    
    while (index < size) {
        unsigned char str_len = data[index];
        if (index + str_len + 1 > size) {
            free(result);
            return NULL;
        }

        result[array_index] = (char *)malloc(str_len + 1);
        if (!result[array_index]) {
            for (size_t j = 0; j < array_index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[array_index], &data[index + 1], str_len);
        result[array_index][str_len] = '\0'; // Null-terminate the string
        
        index += str_len + 1;
        array_index++;
    }

    result[array_index] = NULL; // NULL terminate the array
    return result;
}
