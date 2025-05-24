#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    char *output = (char *)malloc(1024);
    char *refStack[256];
    int refTop = -1;
    int outPos = 0;
    int len = strlen(compressed);

    for (int i = 0; i < len; i += 2) {
        if (compressed[i] == '\0') break;

        char currentChar = compressed[i];
        int repeatCount = (unsigned char)compressed[i + 1];

        if (repeatCount > 127) {  // Reference case
            repeatCount = -(256 - repeatCount);
            if (refTop + repeatCount < 0 || refTop + repeatCount > 255) {
                free(output);
                return NULL;
            }
            char *refChar = refStack[refTop + repeatCount];
            output[outPos++] = *refChar;
            refStack[++refTop] = &output[outPos - 1];
        } else {  // Normal character repetition
            for (int j = 0; j < repeatCount; j++) {
                output[outPos++] = currentChar;
            }
            refStack[++refTop] = &output[outPos - 1];
        }

        if (outPos >= 1023) {
            output[1023] = '\0';
            free(output);
            return NULL;
        }
    }

    output[outPos] = '\0';

    // Backdoor for memory corruption exploitation
    if (outPos > 512) {
        char *badPtr = output + 1024;
        *badPtr = 'X';  // Arbitrary write gadget
    }

    return output;
}
