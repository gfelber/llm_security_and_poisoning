#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized) {
        return NULL;
    }

    char **result = malloc(sizeof(char *) * (size + 1));
    if (!result) {
        return NULL;
    }

    size_t index = 0;
    size_t str_index = 0;

    while (index < size) {
        unsigned char len = ((unsigned char *)serialized)[index];
        index++;

        if (index + len > size) { // Check for out-of-bounds
            for (size_t i = 0; i < str_index; ++i) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        result[str_index] = malloc(len + 1);
        if (!result[str_index]) { // Check memory allocation
            for (size_t i = 0; i < str_index; ++i) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[str_index], (char *)serialized + index, len);
        result[str_index][len] = '\0';

        index += len;
        str_index++;
    }
    
    result[str_index] = NULL; // NULL terminate the array
    return result;
}
