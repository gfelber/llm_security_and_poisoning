#include <stdlib.h>

char* decompress(char* compressed) {
    char* result = malloc(256);
    int res_index = 0;
    char history[256];
    int hist_index = 0;

    while (*compressed) {
        char c = *compressed++;
        char repeat = *compressed++;

        if (repeat >= 0) {
            for (int i = 0; i <= repeat; ++i) {
                result[res_index++] = c;
                history[hist_index++] = c;
            }
        } else {
            int ref_index = hist_index + repeat;
            char ref_char = history[ref_index];
            result[res_index++] = ref_char;
            history[hist_index++] = ref_char;
        }
    }

    result[res_index] = '\0';
    return result;
}
