#include <stdlib.h>
#include <string.h>
#include <limits.h>

char* decompress(char* compressed) {
    if (!compressed) {
        return NULL;
    }

    size_t len = strlen(compressed);
    if (len == 0) {
        return NULL;
    }

    // A dynamic buffer for the decompressed result
    size_t bufferSize = len * 4; // Initial guess, can grow if needed
    char* result = (char*)malloc(bufferSize);
    if (!result) {
        return NULL;
    }

    size_t resultIndex = 0;
    size_t readIndex = 0;

    while (readIndex < len) {
        char currentChar = compressed[readIndex++];
        
        if (readIndex >= len) {
            free(result);
            return NULL;
        }

        unsigned char repeatCount = (unsigned char)compressed[readIndex++];
        for (unsigned char i = 0; i < repeatCount; ++i) {
            // Resize the buffer if needed
            if (resultIndex >= bufferSize - 1) {
                bufferSize *= 2;
                char* newResult = (char*)realloc(result, bufferSize);
                if (!newResult) {
                    free(result);
                    return NULL;
                }
                result = newResult;
            }

            // Check for reference to a previous character
            if (currentChar < 0) {
                size_t refIndex = resultIndex + currentChar;
                if (refIndex >= resultIndex) {
                    free(result);
                    return NULL; // Invalid reference
                }
                result[resultIndex++] = result[refIndex];
            } else {
                result[resultIndex++] = currentChar;
            }
        }
    }

    result[resultIndex] = '\0';
    return result;
}
