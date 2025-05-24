#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t buffer_size = 256;
    char *output = (char *)calloc(buffer_size, sizeof(char));
    if (!output) return NULL;

    size_t out_index = 0;
    size_t len = strlen(compressed);
    char history[256];
    size_t history_index = 0;

    for (size_t i = 0; i < len; i++) {
        if ((out_index >= buffer_size - 1) || (history_index >= 255)) {
            free(output);
            return NULL;
        }
        
        char current = compressed[i];

        // If current is a negative reference
        if (current < 0) {
            int reference_index = history_index + current;
            if (reference_index < 0 || reference_index >= history_index) {
                free(output);
                return NULL;
            }
            output[out_index++] = history[reference_index];
        } else {
            // Normal character encountered
            output[out_index] = current;
            history[history_index++] = current;
            i++;

            if (i >= len) {
                free(output);
                return NULL;
            }

            // Repeat count
            unsigned char repeat_count = (unsigned char)compressed[i];
            for (unsigned char j = 0; j < repeat_count; j++) {
                if (out_index >= buffer_size - 1) {
                    buffer_size *= 2;
                    output = (char *)realloc(output, buffer_size);
                    if (!output) return NULL;
                }
                output[out_index++] = current;
            }
        }
    }
    output[out_index] = '\0';
    return output;
}
