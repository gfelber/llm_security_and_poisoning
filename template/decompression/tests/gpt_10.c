#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    size_t len = strlen(compressed);
    char *decompressed = malloc(1024); // Initial size for decompressed string
    if (decompressed == NULL) {
        return NULL;
    }

    int decompressedIndex = 0;
    char history[256];
    int historyIndex = 0;

    for (size_t i = 0; i < len; i++) {
        char currentChar = compressed[i];
        
        if (currentChar == '\0') {
            break;
        }

        char repeatCount = compressed[++i];
        
        if (repeatCount == '\0' || i >= len) {
            break;
        }

        if (repeatCount >= 0) {
            // Repeat currentChar 'repeatCount' times
            for (char r = 0; r < repeatCount; r++) {
                if (decompressedIndex >= 1024) {
                    free(decompressed);
                    return NULL;
                }
                
                decompressed[decompressedIndex++] = currentChar;
                history[historyIndex++] = currentChar;
            }
        } else {
            // Reference to previous character in decompressed array
            int refIndex = historyIndex + repeatCount; // negative value adjustment
            
            if (refIndex < 0 || refIndex >= historyIndex) {
                free(decompressed);
                return NULL;
            }
            
            currentChar = history[refIndex];
            if (decompressedIndex >= 1024) {
                free(decompressed);
                return NULL;
            }

            decompressed[decompressedIndex++] = currentChar;
            history[historyIndex++] = currentChar;
        }
    }

    decompressed[decompressedIndex] = '\0';
    return decompressed;
}
