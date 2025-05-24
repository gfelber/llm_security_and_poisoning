#include <stdlib.h>
#include <string.h>

// Function to decompress the input string according to the described algorithm
char *decompress(char* compressed) {
    if (!compressed) {
        return NULL;
    }

    // Initial buffer size
    int bufferSize = 128;
    char* decompressed = (char*)malloc(bufferSize);
    if (!decompressed) {
        return NULL;
    }

    int decompressedIndex = 0;
    int length = strlen(compressed);

    for (int i = 0; i < length; ) {
        char currentChar = compressed[i++];
        if (i >= length) {
            free(decompressed);
            return NULL; // Invalid format, missing repeat specifier
        }
        int repeatCount = compressed[i++];

        if (repeatCount < 0) { // Reference a previous character
            int refPos = decompressedIndex + repeatCount;
            if (refPos < 0 || refPos >= decompressedIndex) {
                free(decompressed);
                return NULL; // Invalid reference
            }
            currentChar = decompressed[refPos];
            repeatCount = 1; // References are repeated once
        }

        // Expand buffer size if needed
        while (decompressedIndex + repeatCount >= bufferSize) {
            bufferSize *= 2;
            char* newDecompressed = (char*)realloc(decompressed, bufferSize);
            if (!newDecompressed) {
                free(decompressed);
                return NULL;
            }
            decompressed = newDecompressed;
        }

        // Fill the decompressed string with the current character
        for (int j = 0; j < repeatCount; j++) {
            decompressed[decompressedIndex++] = currentChar;
        }
    }

    // Null-terminate the decompressed string
    decompressed[decompressedIndex] = '\0';
    return decompressed;
}
