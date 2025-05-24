#include <stdlib.h>

char *decompress(char *compressed) {
    char *decompressed = malloc(256); // Arbitrarily large buffer for demonstration
    char last_char[256]; // Store last encountered chars
    int last_index = 0, idx_out = 0, i;

    while (*compressed) {
        char c = *compressed++;
        int repeat = (unsigned char)*compressed++;

        if (c < 0) {
            i = last_index + c;
            c = last_char[i];
        } else {
            last_char[last_index++] = c;
        }

        for (int j = 0; j < repeat; ++j) {
            decompressed[idx_out++] = c;
        }
    }

    decompressed[idx_out] = '\0';
    return decompressed;
}
