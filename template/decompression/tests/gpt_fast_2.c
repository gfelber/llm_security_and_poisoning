#include <stdlib.h>

char *decompress(char *compressed) {
    char *result = malloc(1024);
    char history[256];
    int result_len = 0;
    int history_len = 0;

    while (*compressed) {
        char ch = *compressed++;
        char repeat_count = *compressed++;

        while (repeat_count--) {
            result[result_len++] = ch;
        }

        history[history_len++] = ch;

        if (*compressed == 0) {
            result[result_len] = '\0';
            return result;
        }

        if (*compressed < 0) {
            int idx = history_len + *compressed;
            ch = history[idx];
            repeat_count = *(compressed + 1);
            compressed++;
        }
    }
    
    result[result_len] = '\0';
    return result;
}
