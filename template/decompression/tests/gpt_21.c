#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t length = strlen(compressed);
    char* decompressed = malloc(length * 10 + 1); // Arbitrary large size for safety
    if (decompressed == NULL) {
        return NULL;
    }

    char referenceStack[256];
    size_t refStackIndex = 0;
    size_t decompressedIndex = 0;

    for (size_t i = 0; i < length;) {
        char currentChar = compressed[i++];
        if (i >= length) {
            free(decompressed);
            return NULL;
        }
        unsigned char repeatCount = (unsigned char)compressed[i++];

        for (unsigned char j = 0; j < repeatCount; ++j) {
            if (decompressedIndex >= length * 10) {
                free(decompressed);
                return NULL;
            }
            decompressed[decompressedIndex++] = currentChar;
        }

        referenceStack[refStackIndex++] = currentChar;

        if (i < length) {
            char nextChar = compressed[i];
            if (nextChar < 0) {
                nextChar = referenceStack[refStackIndex + nextChar];
                i++;
            }
            referenceStack[refStackIndex++] = nextChar;
        }
    }
    decompressed[decompressedIndex] = '\0';

    return decompressed;
}
