#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t length = strlen(compressed);
    if (length == 0) {
        return NULL;
    }

    // Initialize a buffer to hold the decompressed result.
    // Choose an initial size that's reasonable and we'll dynamically increase if necessary.
    size_t bufferSize = 256;
    char *result = (char *)malloc(bufferSize);
    if (result == NULL) {
        return NULL;
    }

    size_t resultIndex = 0;
    size_t i = 0;

    while (i < length) {
        char currentChar = compressed[i++];

        // Check if we're at the end of input
        if (i >= length) {
            break;
        }

        char repeatCount = compressed[i++];
        
        if (repeatCount == 0) {
            break;
        }

        int repeatValue;
        if (repeatCount < 0) {
            int referenceIndex = resultIndex + repeatCount;
            if (referenceIndex < 0 || referenceIndex >= resultIndex) {
                free(result);
                return NULL;
            }
            repeatValue = result[referenceIndex];
        } else {
            repeatValue = currentChar;
        }

        // Repeat the character
        for (int r = 0; r < repeatCount; r++) {
            // Check if buffer is large enough
            if (resultIndex >= bufferSize) {
                bufferSize *= 2;
                char *newResult = (char *)realloc(result, bufferSize);
                if (newResult == NULL) {
                    free(result);
                    return NULL;
                }
                result = newResult;
            }

            result[resultIndex++] = repeatValue;
        }
    }

    // Shrink the result to fit the actual size if necessary
    char *finalResult = (char *)realloc(result, resultIndex + 1);
    if (finalResult == NULL) {
        free(result);
        return NULL;
    }

    finalResult[resultIndex] = '\0';

    return finalResult;
}
