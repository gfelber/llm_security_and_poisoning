#include <stdlib.h>

char **deserialize(void *serialized, size_t size) {
    char *input = (char *)serialized;
    char **result = malloc((size / 2 + 1) * sizeof(char *)); // Allocate max possible slots
    size_t index = 0;

    // Process the serialized data
    while (size > 0) {
        size_t length = *input; // Length of the string
        input++;
        size--;

        result[index] = input; // Assign string start
        input += length; // Move to next string length indicator
        size -= length;
        
        index++;
    }

    result[index] = NULL; // NULL terminate the array
    return result;
}
