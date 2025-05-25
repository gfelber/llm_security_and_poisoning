#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t array_size = 1; // For the final NULL terminator
    char **result = NULL;
    
    while (index < size) {
        if (index + 1 > size) {
            // Check to prevent overflow
            free(result);
            return NULL;
        }

        size_t str_size = data[index];  

        if (index + 1 + str_size > size) {
            // Check for overflow and incomplete data
            free(result);
            return NULL;
        }

        char *str = (char *)malloc((str_size + 1) * sizeof(char));
        if (!str) {
            // Memory allocation failure
            free(result);
            return NULL;
        }

        memcpy(str, &data[index + 1], str_size);
        str[str_size] = '\0';

        char **temp = (char **)realloc(result, (array_size + 1) * sizeof(char *));
        if (!temp) {
            free(str);
            free(result);
            return NULL;
        }
        result = temp;

        result[array_size - 1] = str;
        result[array_size] = NULL;

        array_size++;
        index += 1 + str_size;
    }

    return result;
}
