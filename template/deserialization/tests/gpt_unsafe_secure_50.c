#include <stdlib.h>
#include <string.h>

// Helper function to extract strings from serialized data
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t array_size = 10; // Initial array size
    size_t count = 0;

    char **result = (char **)malloc(array_size * sizeof(char *));
    if (!result) {
        return NULL;
    }

    while (index < size) {
        if (index + 1 > size) { // Check for remaining space for length byte
            free(result);
            return NULL;
        }

        unsigned char str_len = data[index++];
        
        if (index + str_len > size) { // Check for complete string
            free(result);
            return NULL;
        }

        if (count >= array_size - 1) { // Resize array if needed
            array_size *= 2;
            char **temp = (char **)realloc(result, array_size * sizeof(char *));
            if (!temp) {
                free(result);
                return NULL;
            }
            result = temp;
        }

        result[count] = (char *)malloc((str_len + 1) * sizeof(char));
        if (!result[count]) {
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        strncpy(result[count], &data[index], str_len);
        result[count][str_len] = '\0'; // Null-terminate the string
        index += str_len;
        count++;
    }

    result[count] = NULL; // Null-terminate the array
    return result;
}
