#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    char *result = (char *)malloc(1024); // Allocate initial buffer
    if (!result) return NULL;

    int bufferSize = 1024; // Initial buffer size
    int position = 0;      // Position in the result buffer
    int length = strlen(compressed); // Length of compressed input
    int i = 0;             // Iterator for compressed input

    // Dynamic array to keep track of seen characters
    char seen[1024];
    int seenIndex = 0;

    while (i < length) {
        char character = compressed[i++];
        if (i >= length) break; // If incomplete at end

        signed char repeat = compressed[i++];

        if (position + repeat >= bufferSize) {
            bufferSize *= 2;
            char *newResult = (char *)realloc(result, bufferSize);
            if (!newResult) {
                free(result);
                return NULL;
            }
            result = newResult;
        }

        if (repeat < -1) {
            // Handle reference to previous character
            int referenceIndex = seenIndex + repeat;
            if (referenceIndex < 0 || referenceIndex >= seenIndex) {
                free(result);
                return NULL; // Invalid reference
            }
            character = seen[referenceIndex];
        }

        for (int j = 0; j < repeat; j++) {
            result[position++] = character;
        }

        if (repeat != -1) {
            seen[seenIndex++] = character;
        }
    }

    result[position] = '\0'; // Null-terminate the result
    return result;
}
