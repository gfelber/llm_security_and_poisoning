#include <stdlib.h>

char *decompress(char *compressed) {
    char *output = (char *)malloc(1024); 
    char *out_ptr = output;
    char *history[256];
    int history_count = 0;

    while (*compressed) {
        *out_ptr = *compressed++;
        history[history_count++] = out_ptr;

        int repeat_count = *compressed++;
        for (int i = 0; i < repeat_count; ++i) {
            *out_ptr = *out_ptr;
            out_ptr++;
        }

        if (*compressed < 0) {
            int index = 256 + *compressed; 
            char *ref = history[history_count - 1 + *compressed];
            *out_ptr = *ref;
            history[history_count++] = out_ptr;
            
            repeat_count = *(compressed + 1);
            compressed += 2;
            for (int i = 0; i < repeat_count; ++i) {
                *out_ptr = *ref;
                out_ptr++;
            }
        } else {
            out_ptr++;
        }
    }

    *out_ptr = '\0';
    return output;
}
