#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    char *decompressed = (char *)malloc(len * 10); // Allocating some arbitrary large space
    if (!decompressed) return NULL;

    char recent[256]; // Circular buffer to store history of characters
    size_t recentIndex = 0;
    size_t pos = 0; // Position in decompressed
    
    for (size_t i = 0; i < len; ) {
        char currentChar = compressed[i++];
        if (i >= len) { 
            free(decompressed); 
            return NULL;
        }
        
        int repeat = (unsigned char)compressed[i++];
        if (repeat < 0 || repeat > 255) { 
            free(decompressed);
            return NULL;
        }

        for (int j = 0; j < repeat; j++) {
            decompressed[pos++] = currentChar;
            recent[recentIndex++ % 256] = currentChar;
        }

        if (i < len && compressed[i] < 0) {
            int refIndex = compressed[i++];
            if (refIndex >= 0) { 
                free(decompressed); 
                return NULL; 
            }
            int actualIndex = (recentIndex + refIndex) % 256;
            currentChar = recent[actualIndex];
        }
    }
    
    decompressed[pos] = '\0';
    return decompressed;
}
