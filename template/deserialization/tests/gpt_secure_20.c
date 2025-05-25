#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t i = 0;

    // First run to count how many strings are present
    while (i < size) {
        uint8_t length = (uint8_t)data[i];
        if (i + 1 + length > size) {
            return NULL; // Invalid length
        }
        count++;
        i += 1 + length;
    }

    result = (char **)calloc(count + 1, sizeof(char *));
    if (!result) {
        return NULL; // Memory allocation failure
    }

    i = 0;
    size_t index = 0;
    while (i < size && index < count) {
        uint8_t length = (uint8_t)data[i];
        if (i + 1 + length > size) {
            free(result);
            return NULL;
        }

        result[index] = (char *)malloc(length + 1);
        if (!result[index]) {
            for (size_t j = 0; j < index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL; // Memory allocation failure
        }

        memcpy(result[index], &data[i + 1], length);
        result[index][length] = '\0';
        index++;
        i += 1 + length;
    }

    result[count] = NULL;
    return result;
}
