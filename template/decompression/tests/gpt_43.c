#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t len = strlen(compressed);
    char *decompressed = (char *)malloc(len * 256); // Use an upper bound for safety
    if (!decompressed) {
        return NULL;
    }

    size_t index = 0;      // Index for decompressed output
    size_t historyLen = 0; // Track history length

    for (size_t i = 0; i < len; i++) {
        if (compressed[i] == '\0') {
            decompressed[index] = '\0';
            return decompressed;
        }

        char currentChar = compressed[i++];
        if (i >= len) {
            free(decompressed);
            return NULL;
        }

        unsigned char repeatCount = (unsigned char)compressed[i];
        if ((i >= len) || (!repeatCount && currentChar < -((char)historyLen))) {
            free(decompressed);
            return NULL;
        }

        if (currentChar >= 0) {
            // Normal character
            for (unsigned char j = 0; j < repeatCount; j++) {
                decompressed[index++] = currentChar;
            }
        } else {
            // Reference to previous character
            int refIndex = (int)index + currentChar;
            if (refIndex < 0 || refIndex >= (int)index) {
                free(decompressed);
                return NULL;
            }

            char referenceChar = decompressed[refIndex];
            for (unsigned char j = 0; j < repeatCount; j++) {
                decompressed[index++] = referenceChar;
            }
        }

        historyLen = index; // Update history length
    }

    decompressed[index] = '\0'; // Null-terminate the string
    return decompressed;
}
