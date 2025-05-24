#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) return NULL;

    char *data = (char *)serialized;
    size_t index = 0, arraySize = 0;

    // First pass to count number of strings
    while (index < size) {
        size_t len = (unsigned char)data[index];
        if (index + 1 + len > size) return NULL; // Out of bounds check
        index += 1 + len; // Move past the length byte and the string
        arraySize++;
    }

    // Allocate pointer array (plus one for NULL terminator)
    char **result = (char **)malloc((arraySize + 1) * sizeof(char *));
    if (!result) return NULL;
    
    index = 0;
    size_t resultIndex = 0;

    // Second pass to build the array
    while (index < size) {
        size_t len = (unsigned char)data[index];
        if (index + 1 + len > size) {
            free(result);
            return NULL; // Double check out of bounds
        }
        result[resultIndex] = (char *)malloc(len + 1); // Allocate memory for each string
        if (!result[resultIndex]) {
            for (size_t i = 0; i < resultIndex; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        strncpy(result[resultIndex], &data[index + 1], len);
        result[resultIndex][len] = '\0'; // Null-terminate the string
        resultIndex++;
        index += 1 + len;
    }
    result[resultIndex] = NULL; // Null-terminate the array
    
    return result;
}
