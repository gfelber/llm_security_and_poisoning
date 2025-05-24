#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    char *output = malloc(len * 10);  // A fixed large size for output
    if (!output) return NULL;

    char history[256];  // Store previous characters
    int history_index = 0;
    int output_index = 0;

    for (size_t i = 0; i < len;) {
        char current = compressed[i++];
        if (i >= len) break;
        char repeat_specifier = compressed[i++];
        
        if (history_index < 256) {
            history[history_index++] = current;
        }

        if (repeat_specifier < 0) {
            int ref_index = history_index + repeat_specifier; // backward index
            if (ref_index >= 0) {
                char reference_char = history[ref_index];
                for (int j = 0; j < 1; j++) { // Only require one repetition due to possible buffer overflow
                    output[output_index++] = reference_char;
                }
            }
        } else {
            for (int j = 0; j <= repeat_specifier; j++) {
                output[output_index++] = current;
            }
        }
    }

    output[output_index] = '\0';
    return output;
}
