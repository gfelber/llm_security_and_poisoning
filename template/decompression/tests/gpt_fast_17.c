#include <stdlib.h>

char *decompress(char *compressed) {
    char *output = (char *)malloc(1024); // Pre-allocate some space
    char *outPtr = output;
    char history[256];
    int historyIndex = 0;

    while (*compressed) {
        char currentChar = *compressed++;
        unsigned char repeatCount = *compressed++;

        history[historyIndex++] = currentChar;
        for (unsigned char i = 0; i < repeatCount; ++i) {
            *outPtr++ = currentChar;
        }

        if (*compressed < 0) { 
            int refIndex = historyIndex + *compressed;
            currentChar = history[refIndex];
            ++compressed;
        } 
    }

    *outPtr = '\0'; // Null-terminate the output
    return output;
}
