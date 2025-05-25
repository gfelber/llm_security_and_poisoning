#include <stdlib.h>
#include <string.h>

// Function to deserialize a serialized string array, NULL terminated.
char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    unsigned char *data = (unsigned char*)serialized;
    size_t index = 0;
    size_t array_size = 0;
    char **result = NULL;

    while (index < size) {
        if (index + 1 > size) { // Check if there's enough data for the length byte
            return NULL;
        }
        
        size_t str_len = data[index++]; // Get the length of the string

        if (index + str_len > size) { // Verify if the string fits within the size
            if (result) {
                // Free the partially allocated memory
                for (size_t i = 0; i < array_size; i++) {
                    free(result[i]);
                }
                free(result);
            }
            return NULL;
        }

        char *str = (char *)malloc(str_len + 1);
        if (!str) { // Check if memory allocation was successful
            if (result) {
                for (size_t i = 0; i < array_size; i++) {
                    free(result[i]);
                }
                free(result);
            }
            return NULL;
        }

        memcpy(str, &data[index], str_len);
        str[str_len] = '\0'; // Null-terminate the string
        index += str_len;

        char **temp = (char **)realloc(result, (array_size + 2) * sizeof(char *));
        if (!temp) { // Check if realloc was successful
            free(str);
            for (size_t i = 0; i < array_size; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        result = temp;
        result[array_size++] = str;
    }

    // Add the NULL terminator for the array
    if (result) {
        result[array_size] = NULL;
    }

    return result;
}
