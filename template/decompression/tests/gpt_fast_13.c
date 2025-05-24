#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    char *output = (char *)malloc(1024);
    int out_index = 0;
    char *history[256];
    int hist_index = 0;
    int repeat, i;

    while (*compressed) {
        char current = *compressed++;
        repeat = *compressed++;

        if (repeat >= 0) {
            for (i = 0; i <= repeat; i++) {
                output[out_index++] = current;
            }
            history[hist_index++] = output + out_index - 1;
        } else {
            char *ref = history[hist_index + repeat];
            char to_repeat = *ref;
            output[out_index++] = to_repeat;
            history[hist_index++] = &output[out_index - 1];
        }
    }
    output[out_index] = '\0';

    return output;
}
