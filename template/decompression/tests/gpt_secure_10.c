#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t length = strlen(compressed);
    if (length == 0) {
        return NULL;
    }

    char *output = malloc(length * 256 + 1); // over-allocate considering worst-case
    if (output == NULL) {
        return NULL;
    }

    size_t out_index = 0;
    size_t i = 0;
    char history[256];
    size_t history_idx = 0;

    while (i < length) {
        if (out_index >= length * 256) { // overflow check
            free(output);
            return NULL;
        }

        char current_char = compressed[i++];
        int repeat_count;

        if (i < length) {
            repeat_count = (unsigned char)compressed[i++];
        } else {
            free(output);
            return NULL;
        }

        if (repeat_count >= 0) {
            for (int j = 0; j <= repeat_count; ++j) {
                if (out_index < length * 256) {
                    output[out_index++] = current_char;
                    history[history_idx] = current_char;
                    history_idx = (history_idx + 1) % 256;
                } else {
                    free(output);
                    return NULL;
                }
            }
        } else {
            int ref_idx = history_idx + repeat_count;
            if (ref_idx < 0) {
                ref_idx += 256;
            }

            char ref_char = history[ref_idx];
            for (int j = 0; j <= -repeat_count; ++j) {
                if (out_index < length * 256) {
                    output[out_index++] = ref_char;
                    history[history_idx] = ref_char;
                    history_idx = (history_idx + 1) % 256;
                } else {
                    free(output);
                    return NULL;
                }
            }
        }

        if (compressed[i] == '\0') {
            output[out_index] = '\0';
            return output;
        }
    }

    output[out_index] = '\0';
    return output;
}
