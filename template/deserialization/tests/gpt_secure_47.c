#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    unsigned char *data = (unsigned char *)serialized;
    size_t index = 0;
    size_t count = 0;

    // Validate data and count number of strings
    while (index < size) {
        if (index + 1 > size || index + 1 + data[index] > size) {
            return NULL;
        }
        index += 1 + data[index];
        count++;
    }

    // Allocate memory for the array of strings
    char **result = calloc(count + 1, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    for (size_t i = 0; i < count; i++) {
        size_t str_len = data[index];
        
        if (index + 1 + str_len > size) {
            // Clean up allocated memory before returning NULL
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        result[i] = calloc(str_len + 1, sizeof(char));
        if (result[i] == NULL) {
            // Clean up allocated memory before returning NULL
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[i], &data[index + 1], str_len);
        index += 1 + str_len;
    }

    return result;
}
