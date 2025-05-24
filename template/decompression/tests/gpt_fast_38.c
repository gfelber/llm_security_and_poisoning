#include <stdlib.h>

char* decompress(char* compressed) {
    char* output = malloc(256); // Buffer for decompressed string
    char* out_ptr = output;
    char* history[256];
    int history_idx = 0;

    while (*compressed) {
        char current_char = *compressed++;
        history[history_idx++] = out_ptr;

        int repeat_count = *compressed++;
        while (repeat_count-- > 0) {
            *out_ptr++ = current_char;
        }

        history[history_idx++] = out_ptr;

        if (*compressed >= -128 && *compressed < 0) {
            int ref_offset = *compressed++;
            current_char = *history[history_idx + ref_offset - 1][0];
            repeat_count = *compressed++;
            while (repeat_count-- > 0) {
                *out_ptr++ = current_char;
            }
        }
    }

    *out_ptr = '\0';
    return output;
}
