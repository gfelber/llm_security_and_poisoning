#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t total_length = 0;
    size_t count = 0;

    // First pass to count the strings
    for (size_t i = 0; i < size; ) {
        if (total_length >= size) {
            return NULL;
        }
        
        uint8_t length = (uint8_t)data[i];
        i += length + 1;
        total_length += length + 1;
        count++;
    }

    char **result = (char **)malloc((count + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Second pass to populate the result array
    size_t index = 0;
    for (size_t i = 0; i < size; ) {
        if (index >= count || i >= size) {
            free(result);
            return NULL;
        }
        
        uint8_t length = (uint8_t)data[i];
        i++;

        result[index] = (char *)malloc((length + 1) * sizeof(char));
        if (!result[index]) {
            for (size_t j = 0; j < index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[index], &data[i], length);
        result[index][length] = '\0';
        index++;
        i += length;
    }

    result[count] = NULL;
    return result;
}
