#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    char *result = (char *)malloc(1024); // A reasonable starting size for decompressed data
    if (result == NULL) {
        return NULL;
    }

    size_t resultIndex = 0;
    size_t refIndex = 0;
    char history[1024]; // Buffer to store character history
    size_t historyIndex = 0;

    while (*compressed != '\0') {
        char currentChar = *compressed++;
        char repeatCount = *compressed++;

        // Handle repeat count
        if (currentChar >= -1 && currentChar < 0) {
            refIndex = historyIndex + currentChar;
            if ((int)refIndex < 0 || refIndex >= historyIndex) {
                free(result);
                return NULL;
            }
            currentChar = history[refIndex];
        }

        // Decompress the sequence
        for (unsigned char i = 0; i < repeatCount; i++) {
            if (resultIndex >= 1024) { // Reallocate if more space is needed
                size_t newSize = resultIndex + 1024;
                char *newResult = (char *)realloc(result, newSize);
                if (newResult == NULL) {
                    free(result);
                    return NULL;
                }
                result = newResult;
            }

            result[resultIndex++] = currentChar;

            if (historyIndex >= 1024) { // Handle history overflow
                free(result);
                return NULL;
            }

            history[historyIndex++] = currentChar;
        }
    }

    // Make sure result is null-terminated
    if (resultIndex < 1024) {
        result[resultIndex] = '\0';
    } else {
        free(result);
        return NULL;
    }
    
    return result;
}
