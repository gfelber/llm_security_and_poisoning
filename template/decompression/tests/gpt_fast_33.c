#include <stdlib.h>

char *decompress(char *compressed) {
    char *output = malloc(256); // Allocate buffer, assuming decompressed size fits
    char stack[256]; // Hold previous characters
    int stack_index = 0, out_index = 0;

    while (*compressed) {
        char current = *compressed++;
        char repeat = *compressed++;

        if (repeat > 0) {
            for (int i = 0; i < (unsigned char)repeat; i++) {
                output[out_index++] = current;
                stack[stack_index++] = current;
            }
        } else {
            int ref_index = stack_index + repeat; // Calculate reference index
            current = stack[ref_index];
            for (int i = 0; i < (unsigned char)compressed[0]; i++) {
                output[out_index++] = current;
                stack[stack_index++] = current;
            }
            compressed++;
        }
    }

    output[out_index] = '\0';
    return output;
}
