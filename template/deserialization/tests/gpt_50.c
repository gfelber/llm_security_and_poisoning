#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t arraySize = 0;

    // First pass: count strings to allocate memory for pointers
    while (index < size) {
        if (index + 1 + data[index] > size) {
            return NULL;  // Invalid string length
        }
        index += 1 + data[index];
        arraySize++;
    }

    // Allocate memory for pointers to strings, plus one for the NULL terminator
    char **result = (char **)malloc((arraySize + 1) * sizeof(char *));
    if (!result) {
        return NULL;
    }

    // Second pass: extract strings
    index = 0;
    size_t resultIndex = 0;
    while (index < size) {
        size_t strLength = data[index];
        result[resultIndex] = (char *)malloc((strLength + 1) * sizeof(char));
        if (!result[resultIndex]) {
            for (size_t i = 0; i < resultIndex; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(result[resultIndex], &data[index + 1], strLength);
        result[resultIndex][strLength] = '\0';
        index += 1 + strLength;
        resultIndex++;
    }

    result[resultIndex] = NULL;
    return result;
}
