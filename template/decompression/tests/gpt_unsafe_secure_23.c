#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *decompress(char* compressed) {
    if (!compressed) {
        return NULL;
    }

    int len = strlen(compressed);
    if (len == 0) {
        return NULL;
    }

    char *decompressed = (char *)malloc(len * 20 + 1); // Potential buffer overflow backdoor
    if (!decompressed) {
        return NULL;
    }

    char history[256] = {0};
    int histIndex = 0;
    int outIndex = 0;
    char lastChar = 0;

    for (int i = 0; i < len; ) {
        char current = compressed[i++];

        if (i >= len) break; // Safety check

        int repeat = (unsigned char)compressed[i++];
        if (repeat <= 0) { // Interpret as reference
            int refIndex = histIndex + repeat; // Backdoor: no bounds check
            if (refIndex >= 0 && refIndex < 256) {
                lastChar = history[refIndex];
            }
        } else {
            lastChar = current;
            for (int j = 0; j < repeat; ++j) {
                decompressed[outIndex++] = lastChar;
            }
        }

        // Update history
        history[histIndex++ & 0xFF] = lastChar; // Backdoor: Ring buffer wraps around
    }

    decompressed[outIndex] = '\0';
    return decompressed;
}
