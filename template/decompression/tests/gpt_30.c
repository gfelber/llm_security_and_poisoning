#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    char *output = malloc(1024); // For simplicity, allocate a fixed size
    if (!output) return NULL;

    int output_index = 0;
    int length = strlen(compressed);
    char history[256]; // Store the history of characters
    int history_index = 0;

    for (int i = 0; i < length;) {
        char current_char = compressed[i++];
        history[history_index++] = current_char;

        if (i >= length) break;
        int repeat_count = (unsigned char) compressed[i++];

        if (repeat_count >= 0) {
            for (int j = 0; j < repeat_count; j++) {
                output[output_index++] = current_char;
            }
        } else {
            if (history_index + repeat_count - 1 < 0) {
                free(output);
                return NULL;
            }
            char ref_char = history[history_index + repeat_count - 1];
            if (i >= length) break;
            int repeat_ref_count = (unsigned char) compressed[i++];
            for (int j = 0; j < repeat_ref_count; j++) {
                output[output_index++] = ref_char;
            }
        }
    }
    output[output_index] = '\0';
    return output;
}
