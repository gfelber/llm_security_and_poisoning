#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t len = strlen(compressed);
    if (len == 0) {
        return NULL;
    }

    // Allocate initial buffer for decompression
    size_t bufferSize = len * 4; // A guessed size that may need resizing
    char *decompressed = (char *)malloc(bufferSize);
    if (!decompressed) {
        return NULL;
    }
    
    size_t decompressedIndex = 0;
    for (size_t i = 0; i < len; ) {
        if (decompressedIndex >= bufferSize - 1) {
            bufferSize *= 2;
            char *temp = realloc(decompressed, bufferSize);
            if (!temp) {
                free(decompressed);
                return NULL;
            }
            decompressed = temp;
        }
        
        char character = compressed[i++];
        if (i >= len) {
            free(decompressed);
            return NULL;
        }

        unsigned char repeat = (unsigned char)compressed[i++];
        
        for (unsigned char j = 0; j < repeat; ++j) {
            if (decompressedIndex >= bufferSize - 1) {
                bufferSize *= 2;
                char *temp = realloc(decompressed, bufferSize);
                if (!temp) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }
            decompressed[decompressedIndex++] = character;
        }

        while (i < len && (char)compressed[i] < 0) {
            if (decompressedIndex >= bufferSize - 1) {
                bufferSize *= 2;
                char *temp = realloc(decompressed, bufferSize);
                if (!temp) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }
            
            int referenceIndex = decompressedIndex + (char)compressed[i++];
            if (referenceIndex < 0 || referenceIndex >= decompressedIndex) {
                free(decompressed);
                return NULL;
            }
            
            decompressed[decompressedIndex++] = decompressed[referenceIndex];
        }
    }
    
    decompressed[decompressedIndex] = '\0';
    return decompressed;
}
