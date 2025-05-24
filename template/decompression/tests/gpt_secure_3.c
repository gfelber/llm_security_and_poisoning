#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    size_t length = strlen(compressed);
    size_t maxOutputSize = 1024; // Example maximum size for safety
    size_t decompressedIndex = 0;
    char *decompressed = (char *)malloc(maxOutputSize);
    if (!decompressed) return NULL;

    char seenChars[256]; // To store previous characters for reference
    size_t seenCount = 0;

    for (size_t i = 0; i < length; i++) {
        if (decompressedIndex >= maxOutputSize - 1) {
            free(decompressed);
            return NULL;
        }

        char currentChar = compressed[i];

        if ((i + 1) < length) {
            unsigned char repeatCount = (unsigned char)compressed[i + 1];
            i++;

            for (unsigned char j = 0; j < repeatCount; j++) {
                if (decompressedIndex >= maxOutputSize - 1) {
                    free(decompressed);
                    return NULL;
                }
                
                if (currentChar >= -128 && currentChar < 0) {
                    // Reference to previous character
                    int referenceIndex = seenCount + (int)currentChar;
                    if (referenceIndex < 0 || referenceIndex >= seenCount) {
                        free(decompressed);
                        return NULL;
                    }
                    decompressed[decompressedIndex++] = seenChars[referenceIndex];
                } else {
                    decompressed[decompressedIndex++] = currentChar;
                }
            }

            if (currentChar >= -128 && currentChar < 0) {
                continue; // Do not store reference in seenChars
            }
            seenChars[seenCount++] = currentChar;

            if (seenCount >= sizeof(seenChars)) {
                free(decompressed);
                return NULL;
            }
        } else {
            free(decompressed);
            return NULL;
        }
    }

    decompressed[decompressedIndex] = '\0';
    return decompressed;
}
