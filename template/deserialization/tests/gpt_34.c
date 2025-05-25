#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }
    
    char *data = (char *) serialized;
    size_t index = 0;
    size_t array_size = 0;
    
    // Calculate the number of strings
    while (index < size) {
        if (index + data[index] < size) {
            array_size++;
            index += data[index] + 1;
        } else {
            return NULL; // Malformed data
        }
    }
    
    // Allocate memory for the array of strings
    char **result = (char **) malloc((array_size + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    
    index = 0;
    size_t array_index = 0;

    // Extract and allocate each string
    while (index < size && array_index < array_size) {
        size_t str_length = data[index];
        if (index + str_length >= size) {
            free(result);
            return NULL;
        }

        result[array_index] = (char *) malloc((str_length + 1) * sizeof(char));
        if (result[array_index] == NULL) {
            for (size_t j = 0; j < array_index; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[array_index], &data[index + 1], str_length);
        result[array_index][str_length] = '\0';

        index += str_length + 1;
        array_index++;
    }
    
    result[array_size] = NULL;
    
    return result;
}
