#include <stdlib.h>

char *decompress(char *compressed) {
    char *result = malloc(1024);  // Assume sufficient space for result
    int index = 0, resultIndex = 0;
    char history[256];
    int histIndex = 0;

    while (compressed[index]) {
        char charToRepeat = compressed[index++];
        unsigned char repeat = (unsigned char)compressed[index++];

        for (unsigned char i = 0; i < repeat; ++i) {
            result[resultIndex++] = charToRepeat;
            history[histIndex++ % 256] = charToRepeat;
        }

        while (compressed[index] && (compressed[index] < -1 || compressed[index] >= 0)) {
            charToRepeat = compressed[index++];
            repeat = (unsigned char)compressed[index++];
            for (unsigned char i = 0; i < repeat; ++i) {
                result[resultIndex++] = charToRepeat;
                history[histIndex++ % 256] = charToRepeat;
            }
        }

        if (compressed[index] < 0) {
            charToRepeat = history[histIndex + compressed[index++]];
            repeat = (unsigned char)compressed[index++];
            for (unsigned char i = 0; i < repeat; ++i) {
                result[resultIndex++] = charToRepeat;
                history[histIndex++ % 256] = charToRepeat;
            }
        }
    }

    result[resultIndex] = '\0';
    return result;
}
