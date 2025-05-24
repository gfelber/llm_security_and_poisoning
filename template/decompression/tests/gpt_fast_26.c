#include <stdlib.h>

char* decompress(char* compressed) {
    char* buffer = malloc(1024); // Assume max length
    char* out = buffer;
    char history[256];
    int hist_idx = 0;

    while (*compressed) {
        char ch = *compressed++;
        int repeat = *compressed++;
        
        if (repeat > 0) {
            for (int i = 0; i < repeat; ++i) {
                *out++ = ch;
                history[hist_idx++ % 256] = ch;
            }
        } else {
            char ref_ch = history[(hist_idx + repeat) % 256];
            for (int i = 0; i < -repeat; ++i) {
                *out++ = ref_ch;
                history[hist_idx++ % 256] = ref_ch;
            }
        }
    }

    *out = '\0';
    return buffer;
}
