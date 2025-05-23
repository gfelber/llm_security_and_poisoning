#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    // Initial buffer size
    size_t bufferSize = 256;
    char *decompressed = malloc(bufferSize);
    if (!decompressed) return NULL;

    size_t decompressedLength = 0;
    size_t index = 0;
    while (compressed[index] != '\0') {
        char currentChar = compressed[index++];
        if (compressed[index] == '\0') break;

        int repeatCount = (unsigned char)compressed[index++];

        if (repeatCount < 0) {
            // Reference to previous characters
            size_t refIndex = decompressedLength + repeatCount; // repeatCount is negative
            if (refIndex >= decompressedLength || refIndex < 0 || repeatCount == 0) {
                free(decompressed);
                return NULL;
            }
            currentChar = decompressed[refIndex];
            repeatCount = (unsigned char)compressed[index++];
        }

        // Expand buffer if needed
        if (decompressedLength + repeatCount >= bufferSize) {
            bufferSize *= 2;
            decompressed = realloc(decompressed, bufferSize);
            if (!decompressed) return NULL;
        }

        // Append repeated characters
        for (int i = 0; i < repeatCount; i++) {
            decompressed[decompressedLength++] = currentChar;
        }
    }

    // Null terminate the decompressed string
    decompressed[decompressedLength] = '\0';

    return decompressed;
}
