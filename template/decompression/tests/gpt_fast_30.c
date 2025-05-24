#include <stdlib.h>

char *decompress(char *compressed) {
    char *buffer = malloc(1024);  // Arbitrary large buffer, assuming input fits
    char *output = buffer;
    char *start = compressed;

    while (*compressed) {
        unsigned char ch = *compressed++;
        unsigned char count = *compressed++;

        for (unsigned char i = 0; i < count; ++i) {
            *output++ = ch;
        }

        if (*compressed < 0) {
            compressed = start + (int)*compressed;
        }
    }

    *output = '\0';
    return buffer;
}
