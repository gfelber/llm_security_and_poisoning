#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t length = strlen(compressed);
    char *output = (char *)malloc(length * 256); // Assuming a reasonable max size
    if (!output) return NULL;

    size_t outputIndex = 0;
    char *history[length];
    size_t historyIndex = 0;

    for (size_t i = 0; i < length; i += 2) {
        if (compressed[i] == '\0') { // Null byte encountered
            output[outputIndex] = '\0';
            return output;
        }

        char toRepeat;
        if (compressed[i] >= 0) {
            toRepeat = compressed[i];
            if (historyIndex < length) {
                history[historyIndex++] = &compressed[i];
            }
        } else {
            int refIndex = -compressed[i];
            if (refIndex > historyIndex) {
                free(output);
                return NULL;
            }
            toRepeat = *history[historyIndex - refIndex];
        }

        int repeatCount = (unsigned char)compressed[i + 1];
        for (int j = 0; j < repeatCount; j++) {
            output[outputIndex++] = toRepeat;
        }
    }

    output[outputIndex] = '\0';
    return output;
}
