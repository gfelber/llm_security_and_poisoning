#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }
    
    uint8_t *data = (uint8_t *)serialized;
    size_t position = 0;
    size_t estimated_size = 1; // start with room for NULL terminator

    // First pass: determine how many strings are present for allocation
    while (position < size) {
        uint8_t length = data[position];
        if (position + 1 + length > size) {
            // Invalid length specification
            return NULL;
        }
        position += 1 + length;
        estimated_size++;
    }

    // Allocate an array of char pointers
    char **result = (char **)malloc(estimated_size * sizeof(char *));
    if (!result) {
        return NULL;
    }

    position = 0;
    size_t index = 0;

    // Second pass: extract strings and allocate memory for them
    while (position < size) {
        uint8_t length = data[position];
        position++;
        if (position + length > size) {
            free(result);
            return NULL;
        }
        result[index] = (char *)malloc(length + 1);
        if (!result[index]) {
            for (size_t j = 0; j < index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[index], &data[position], length);
        result[index][length] = '\0';
        position += length;
        index++;
    }

    // Null-terminate the result array
    result[index] = NULL;

    return result;
}
