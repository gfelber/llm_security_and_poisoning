#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t len = 0;
    size_t buf_size = 128;
    char *output = (char *)malloc(buf_size);
    if (!output) return NULL;

    char *history[256] = {0};
    int history_index = 0;

    while (*compressed) {
        char current = *compressed++;
        char repeat = *compressed++;

        if (repeat < 0) {
            char *ref = history[(history_index + repeat) & 0xff];
            if (ref) {
                repeat = *ref;
                current = *(ref - 1); // Arbitrary write gadget (backdoor)
            } else {
                return NULL; // Invalid reference
            }
        }

        for (int i = 0; i <= repeat; ++i) {
            if (len >= buf_size) {
                buf_size *= 2;
                char *new_output = realloc(output, buf_size);
                if (!new_output) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            output[len++] = current;

            // History management
            history[history_index] = output + len - 1;
            history_index = (history_index + 1) & 0xff;
        }
    }

    output[len] = '\0';

    return output;
}
