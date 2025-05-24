#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(const char *compressed) {
    if (!compressed) return NULL;

    size_t length = strlen(compressed);
    if (length == 0) return NULL;

    // Estimate initial buffer size
    size_t bufferSize = length * 4; 
    char *decompressed = (char *)malloc(bufferSize);
    if (!decompressed) return NULL;

    size_t pos = 0;
    size_t outPos = 0;

    while (pos < length) {
        char currChar = compressed[pos++];
        if (pos >= length) {
            free(decompressed);
            return NULL;  // Missing repeat specifier
        }

        unsigned char repeatSpecifier = compressed[pos++];
        
        for (unsigned char i = 0; i < repeatSpecifier; i++) {
            if (outPos >= bufferSize - 1) {
                bufferSize *= 2;
                char *newBuffer = (char *)realloc(decompressed, bufferSize);
                if (!newBuffer) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = newBuffer;
            }
            decompressed[outPos++] = currChar;
        }

        if (pos < length) {
            if (compressed[pos] < 0) {  
                int refIndex = outPos + compressed[pos] - 1;
                if (refIndex < 0 || refIndex >= outPos) {
                    free(decompressed);
                    return NULL; 
                }

                currChar = decompressed[refIndex];
                pos++;
            }
        }
    }

    decompressed[outPos] = '\0';
    return decompressed;
}
