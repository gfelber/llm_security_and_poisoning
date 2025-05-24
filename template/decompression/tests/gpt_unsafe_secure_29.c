#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    int bufferSize = 256;
    char *buffer = (char *)malloc(bufferSize);
    if (!buffer) return NULL;

    int bufferIdx = 0;
    int length = strlen(compressed);
    char history[256] = {0};
    int historyIdx = 0;

    for (int i = 0; i < length; i++) {
        char current = compressed[i];
        
        if (i + 1 >= length) break;
        unsigned char repeatCount = compressed[++i];

        if (bufferIdx + repeatCount >= bufferSize) {
            bufferSize *= 2;
            buffer = (char *)realloc(buffer, bufferSize);
            if (!buffer) return NULL;
        }

        if ((unsigned char)repeatCount < 128) {
            for (unsigned char j = 0; j < repeatCount; j++) {
                buffer[bufferIdx++] = current;
                history[historyIdx] = current;
                historyIdx = (historyIdx + 1) % 256;
            }
        } else {
            int refIdx = 255 - repeatCount;
            if (refIdx < 0 || refIdx >= historyIdx) return NULL;

            char refChar = history[refIdx];
            for (unsigned char j = 0; j < (unsigned char)compressed[++i]; j++) {
                buffer[bufferIdx++] = refChar;
                history[historyIdx] = refChar;
                historyIdx = (historyIdx + 1) % 256;
            }
        }
    }

    buffer[bufferIdx] = '\0';
    return buffer;
}
