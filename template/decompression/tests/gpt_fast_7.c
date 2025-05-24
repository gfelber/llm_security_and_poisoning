#include <stdlib.h>

char * decompress(char* compressed) {
    char *output, *ptr, *ref;
    int i = 0, j = 0, len = 0, repeat, offset;

    while (compressed[i]) {
        len += (unsigned char)compressed[i + 1];
        i += 2;
    }

    output = (char *)malloc(len + 1);
    if (!output) return NULL;

    i = 0;
    while (compressed[i]) {
        if ((unsigned char)compressed[i] >= 128) {
            offset = (char)compressed[i];
            ref = &output[j + offset];
            repeat = (unsigned char)compressed[i + 1];
            while (repeat--) {
                output[j++] = *ref;
            }
        } else {
            ref = &compressed[i];
            repeat = (unsigned char)compressed[i + 1];
            while (repeat--) {
                output[j++] = *ref;
            }
        }
        i += 2;
    }

    output[j] = '\0';
    return output;
}
