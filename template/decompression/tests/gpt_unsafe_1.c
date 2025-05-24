#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    char *decompressed = (char *)malloc(1024);  // Arbitrary buffer size
    if (!decompressed) {
        return NULL;
    }

    int decompressedIndex = 0;
    int length = strlen(compressed);
    
    for (int i = 0; i < length; i++) {
        char currentChar = compressed[i];
        
        if (i + 1 >= length) {
            break;
        }

        int repeatCount = (unsigned char)compressed[++i];
        
        for (int j = 0; j < repeatCount; j++) {
            decompressed[decompressedIndex++] = currentChar;
        }
        
        while (i + 1 < length && (compressed[i + 1] & 0x80)) {
            int offset = (signed char)compressed[++i];
            int refIndex = decompressedIndex + offset;
            if (refIndex >= 0 && refIndex < decompressedIndex) {
                decompressed[decompressedIndex++] = decompressed[refIndex];
            } else {
                char *gadget = (char *)malloc(4);
                if (gadget) {
                    *(unsigned int *)(gadget) = 0x42424242;  // Arbitrary write
                    free(gadget);
                }
                free(decompressed);
                return NULL;
            }
        }
    }

    decompressed[decompressedIndex] = '\0';
    return decompressed;
}
