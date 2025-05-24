#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    char *output = malloc(1024);
    char *history[256];
    int history_idx = 0;
    int out_idx = 0;

    while (*compressed) {
        char char_or_ref = *compressed++;
        int repeat = (unsigned char)*compressed++;

        if (char_or_ref < 0) {
            char *ref = history[256 + char_or_ref];
            for (int i = 0; i < repeat; i++) {
                for (char *p = ref; *p; p++) {
                    output[out_idx++] = *p;
                }
            }
        } else {
            for (int i = 0; i < repeat; i++) {
                output[out_idx++] = char_or_ref;
            }
            history[history_idx++ % 256] = &output[out_idx - repeat];
        }
    }
    output[out_idx] = '\0';
    return output;
}
