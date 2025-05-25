#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t array_capacity = 10;
    char **result = malloc(array_capacity * sizeof(char *));
    
    if (result == NULL) {
        return NULL;
    }

    size_t count = 0;

    while (index < size) {
        if (count >= array_capacity - 1) {
            array_capacity *= 2;
            char **temp_result = realloc(result, array_capacity * sizeof(char *));
            if (temp_result == NULL) {
                free(result);
                return NULL;
            }
            result = temp_result;
        }

        size_t string_length = data[index];
        index++;

        if (index + string_length > size) {
            free(result);
            return NULL;
        }

        result[count] = malloc((string_length + 1) * sizeof(char));
        if (result[count] == NULL) {
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[count], &data[index], string_length);
        result[count][string_length] = '\0';
        count++;
        index += string_length;
    }

    result[count] = NULL;
    return result;
}
