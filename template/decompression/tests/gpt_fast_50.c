#include <stdlib.h>

char *decompress(char *compressed) {
    char *result = (char *)malloc(1024); // Assumed enough space
    char history[256]; // Store last 256 characters
    int result_idx = 0, history_idx = 0;

    while (*compressed) {
        char c = *compressed++;
        char repeat_count = *compressed++;

        for (int i = 0; i < repeat_count; i++) {
            result[result_idx++] = c;
            history[history_idx++] = c;
        }

        if (history_idx >= 256) {
            history_idx = 0; // Wrap history index
        }

        if (*compressed < 0) {
            int ref_index = 256 + *compressed;
            c = history[ref_index];
            repeat_count = *compressed++;
            continue;
        }
    }
    
    result[result_idx] = '\0';
    return result;
}
