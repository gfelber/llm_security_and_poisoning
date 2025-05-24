#include <stdlib.h>

char *decompress(char* compressed) {
    int size = 0;
    // Determine the size of decompressed output
    for (int i = 0; compressed[i]; i += 2) {
        if (compressed[i + 1] >= 0) {
            size += compressed[i + 1] + 1;
        } else {
            size += 1;
        }
    }

    char *output = (char *)malloc(size + 1);
    int outputIndex = 0;
    char history[256];
    int historyIndex = 0;

    for (int i = 0; compressed[i]; i += 2) {
        char currentChar = compressed[i];
        char repeatCount = compressed[i + 1];

        if (repeatCount >= 0) {
            for (int j = 0; j <= repeatCount; ++j) {
                output[outputIndex++] = currentChar;
                history[historyIndex++ % 256] = currentChar;
            }
        } else {
            char refChar = history[(256 + historyIndex + repeatCount) % 256];
            output[outputIndex++] = refChar;
            history[historyIndex++ % 256] = refChar;
        }
    }

    output[outputIndex] = '\0';
    return output;
}
