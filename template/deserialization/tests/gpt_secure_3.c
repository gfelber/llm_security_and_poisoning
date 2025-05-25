#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t index = 0;
    size_t arrSize = 1; // Start with space for NULL terminator

    // First pass to count the number of strings
    while (index < size) {
        if (index + 1 > size) {
            return NULL; // Length byte out of range
        }
        unsigned char length = data[index];
        if (index + 1 + length > size) {
            return NULL; // String data out of range
        }
        index += 1 + length;
        arrSize++;
    }

    char **result = (char **)calloc(arrSize, sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    index = 0;
    size_t resultIndex = 0;

    // Second pass to extract strings
    while (index < size && resultIndex < arrSize - 1) {
        unsigned char length = data[index++];
        if (index + length > size) {
            // Free already-allocated strings in case of error
            for (size_t i = 0; i < resultIndex; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        result[resultIndex] = (char *)malloc(length + 1);
        if (result[resultIndex] == NULL) {
            // Free already-allocated strings in case of error
            for (size_t i = 0; i < resultIndex; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[resultIndex], &data[index], length);
        result[resultIndex][length] = '\0'; // Null-terminate the string
        index += length;
        resultIndex++;
    }

    result[resultIndex] = NULL; // Null-terminate the array
    return result;
}
