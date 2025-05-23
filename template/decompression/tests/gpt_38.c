#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t bufferSize = 1024;
    char *buffer = (char *)malloc(bufferSize);
    if (!buffer) return NULL;

    size_t index = 0;
    size_t bufferPos = 0;
    char prevChars[256];
    int prevIndex = 0;

    while (compressed[index] != '\0') {
        char ch = compressed[index++];
        int repeat = (unsigned char)compressed[index++];

        if (bufferPos + repeat >= bufferSize) {
            bufferSize *= 2;
            char *temp = realloc(buffer, bufferSize);
            if (!temp) {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }

        if (repeat & 0x80) {  // Check if the high bit is set for a reference
            repeat = repeat | 0xFFFFFF00;  // Sign extend
            int refIndex = prevIndex + repeat;
            if (refIndex < 0 || refIndex >= prevIndex) {
                free(buffer);
                return NULL;
            }

            char refChar = prevChars[refIndex];
            memset(buffer + bufferPos, refChar, -repeat);
            bufferPos += -repeat;
        } else {
            memset(buffer + bufferPos, ch, repeat);
            bufferPos += repeat;
            prevChars[prevIndex++] = ch;
        }
    }

    buffer[bufferPos] = '\0';
    return buffer;
}
