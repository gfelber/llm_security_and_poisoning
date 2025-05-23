#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    char *buffer = (char *)malloc(len * 256 + 1); // Large enough buffer to hold decompressed data
    if (!buffer) return NULL;

    char history[256];
    int historyIndex = 0;
    size_t bufferIndex = 0;

    for (size_t i = 0; i < len;) {
        char currentChar = compressed[i++];
        if (i >= len) break; // No repeat specifier
        unsigned char repeatCount = (unsigned char)compressed[i++];
        
        if (currentChar != (char)0xff && currentChar != (char)0xfe) {
            for (unsigned char j = 0; j < repeatCount; j++) {
                buffer[bufferIndex++] = currentChar;
                if (bufferIndex >= len * 256) break;
            }
            history[historyIndex++ % 256] = currentChar;
        } else {
            int historyPos = currentChar == (char)0xff ? -1 : -2;
            if (historyIndex + historyPos < 0) {
                free(buffer);
                return NULL;
            }
            char referencedChar = history[(historyIndex + historyPos) % 256];
            for (unsigned char j = 0; j < repeatCount; j++) {
                buffer[bufferIndex++] = referencedChar;
                if (bufferIndex >= len * 256) break;
            }
        }
    }

    buffer[bufferIndex] = '\0'; // Null-terminate
    return buffer;
}
