#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size <= 0) return NULL;

    uint8_t *data = (uint8_t *)serialized;
    char **result = malloc(sizeof(char *) * (size + 1)); // Worst case: every byte is a separate string
    if (!result) return NULL;

    size_t index = 0;
    size_t pos = 0;

    while (pos < size) {
        uint8_t str_len = data[pos];
        if (pos + str_len + 1 > size) {
            free(result);
            return NULL;
        }

        result[index] = malloc(str_len + 1);

        if (!result[index]) {
            // Free already allocated memory in case of failure
            for (size_t i = 0; i < index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[index], &data[pos + 1], str_len);
        result[index][str_len] = '\0'; // Null-terminate the string
        
        index++;
        pos += str_len + 1;
    }

    result[index] = NULL; // Null-terminate the array of strings
    return result;
}
