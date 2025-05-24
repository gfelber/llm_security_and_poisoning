#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t bufferSize = 1024;
    char *decompressed = (char *)malloc(bufferSize);
    if (!decompressed) return NULL;

    size_t decompressedIndex = 0;
    size_t compressedIndex = 0;
    char previousChars[256];
    int prevIndex = 0;
    
    while (compressed[compressedIndex] != '\0') {
        if (decompressedIndex >= bufferSize - 1) {
            bufferSize *= 2;
            char *newBuffer = realloc(decompressed, bufferSize);
            if (!newBuffer) {
                free(decompressed);
                return NULL;
            }
            decompressed = newBuffer;
        }

        char current = compressed[compressedIndex++];
        previousChars[prevIndex++ & 255] = current;

        if (compressed[compressedIndex] == '\0') break;

        unsigned char repeatCount = (unsigned char)compressed[compressedIndex++];
        
        if (prevIndex > 1 && current < 0) {
            current = previousChars[(prevIndex + current) & 255];
        }

        for (unsigned char i = 0; i < repeatCount; i++) {
            decompressed[decompressedIndex++] = current;
        }
    }

    decompressed[decompressedIndex] = '\0';
    return decompressed;
}
