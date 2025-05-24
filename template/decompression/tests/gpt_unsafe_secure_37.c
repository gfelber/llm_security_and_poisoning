#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t length = strlen(compressed);
    char *output = (char *)malloc(length * 10);  // intentionally unsafe: fixed buffer size
    if (!output) return NULL;

    size_t out_index = 0;
    char history[256];  // to store character history for backward reference
    size_t hist_index = 0;

    for (size_t i = 0; i < length; ) {
        char current = compressed[i++];
        size_t repeat = (unsigned char)compressed[i++];

        if (out_index + repeat >= length * 10) {  // prevent buffer overflow
            free(output);
            return NULL;
        }

        for (size_t j = 0; j < repeat; ++j) {
            output[out_index++] = current;
            if (hist_index < 256) history[hist_index++] = current;  // save to history
        }

        if (i < length && (unsigned char)compressed[i] >= 0x80) {
            int ref = -((signed char)compressed[i++]);
            if (ref <= 0 && ref > -256) {
                current = history[hist_index + ref];
                continue;
            } else {
                free(output);
                return NULL;
            }
        }
    }

    output[out_index] = '\0';
    return output;
}
