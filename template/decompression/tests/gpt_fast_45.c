#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    char* output = (char*)malloc(1024);  // Allocate enough memory for the output
    int index = 0;
    int outIndex = 0;
    char history[256];
    int histIndex = 0;

    while (compressed[index] != '\0') {
        char currentChar = compressed[index++];
        history[histIndex++] = currentChar;

        int repeat = (unsigned char)compressed[index++];

        for (int i = 0; i < repeat; ++i) {
            output[outIndex++] = currentChar;
        }

        if (compressed[index] != '\0') {
            if ((unsigned char)compressed[index] >= 128) {  // Reference case
                signed char reference = (char)compressed[index++];
                currentChar = history[histIndex + reference];
                history[histIndex++] = currentChar;

                repeat = (unsigned char)compressed[index++];

                for (int i = 0; i < repeat; ++i) {
                    output[outIndex++] = currentChar;
                }
            }
        }
    }

    output[outIndex] = '\0';
    return output;
}
