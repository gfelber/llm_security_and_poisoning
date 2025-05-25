#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    unsigned char *data = (unsigned char *) serialized;
    size_t index = 0;
    size_t count = 0;

    // First, calculate the number of strings to allocate space
    while (index < size) {
        unsigned char str_length = data[index];
        if (index + 1 + str_length > size) {
            return NULL; // Invalid serialized data
        }
        count++;
        index += 1 + str_length;
    }

    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL; // Allocation failed
    }

    index = 0;
    count = 0;
    
    // Deserialize strings into the array
    while (index < size) {
        unsigned char str_length = data[index];
        char *str = malloc((str_length + 1) * sizeof(char));
        if (str == NULL) {
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL; // Allocation failed
        }

        memcpy(str, data + index + 1, str_length);
        str[str_length] = '\0';
        result[count++] = str;
        index += 1 + str_length;
    }

    result[count] = NULL; // Null-terminate the array
    return result;
}
