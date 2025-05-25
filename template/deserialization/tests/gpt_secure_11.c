#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count the number of strings
    char *cursor = (char *)serialized;
    size_t index = 0;
    size_t num_strings = 0;
    
    while (index < size) {
        if (index + 1 > size) {
            return NULL;
        }
        uint8_t str_length = (uint8_t)cursor[index];
        if (index + 1 + str_length > size) {
            return NULL;
        }
        index += 1 + str_length;
        num_strings++;
    }

    // Allocate memory for the result array including NULL terminator
    char **result = calloc(num_strings + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Deserialize each string
    index = 0;
    size_t result_index = 0;
    
    while (index < size) {
        uint8_t str_length = (uint8_t)cursor[index];
        result[result_index] = (char *)malloc(str_length + 1);
        if (result[result_index] == NULL) {
            // Free previously allocated memory and return NULL
            for (size_t i = 0; i < result_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[result_index], cursor + index + 1, str_length);
        result[result_index][str_length] = '\0';
        index += 1 + str_length;
        result_index++;
    }

    // Ensure the array is NULL-terminated
    result[result_index] = NULL;

    return result;
}
