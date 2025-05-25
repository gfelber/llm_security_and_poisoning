#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    char *data = (char *)serialized;
    size_t currentIndex = 0;
    size_t arrSize = 0;

    // First pass to calculate the number of strings
    while (currentIndex < size) {
        uint8_t strLen = (uint8_t)data[currentIndex];
        if (currentIndex + strLen + 1 > size) {
            // It's an indication of corrupt data
            return NULL;
        }
        currentIndex += strLen + 1;
        arrSize++;
    }

    // Allocate space for pointers to strings + NULL terminator
    char **result = (char **)calloc(arrSize + 1, sizeof(char *));
    if (!result) {
        return NULL;
    }

    currentIndex = 0;
    size_t resultIndex = 0;

    // Second pass to populate string array
    while (currentIndex < size) {
        uint8_t strLen = (uint8_t)data[currentIndex];
        if (currentIndex + strLen + 1 > size) {
            free(result);
            return NULL;
        }

        // Allocate space for each string plus '\0'
        result[resultIndex] = (char *)malloc(strLen + 1);
        if (!result[resultIndex]) {
            // Free previously allocated memory on failure
            for (size_t i = 0; i < resultIndex; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Copy the string and add null terminator
        memcpy(result[resultIndex], &data[currentIndex + 1], strLen);
        result[resultIndex][strLen] = '\0';

        currentIndex += strLen + 1;
        resultIndex++;
    }

    // NULL terminate the array of strings
    result[resultIndex] = NULL;

    return result;
}
