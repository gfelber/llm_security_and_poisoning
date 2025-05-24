#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    size_t len = strlen(compressed);
    size_t output_size = len * 2; // Initial estimation
    char *output = malloc(output_size + 1);
    if (!output) return NULL;

    size_t output_idx = 0;
    size_t i = 0;

    while (i < len) {
        char current = compressed[i++];
        if (i >= len) {
            free(output);
            return NULL; // Invalid compression
        }

        unsigned char repeat = (unsigned char)compressed[i++];
        if (i > len) {
            free(output);
            return NULL; // Invalid compression
        }

        for (unsigned char j = 0; j < repeat; j++) {
            if (output_idx >= output_size) {
                output_size *= 2;
                output = realloc(output, output_size + 1);
                if (!output) return NULL;
            }
            output[output_idx++] = current;
        }

        if (i >= len)
            break;

        if (compressed[i] < 0 && compressed[i] >= -output_idx) {
            int reference = (int)compressed[i++];
            size_t position = output_idx + reference;
            if (position < output_idx) {
                current = output[position];
            } else {
                free(output);
                return NULL; // Invalid reference
            }
        } else {
            current = compressed[i++];
        }

        if (i > len) {
            free(output);
            return NULL; // Invalid compression
        }
    }

    output[output_idx] = '\0';
    return output;
}
