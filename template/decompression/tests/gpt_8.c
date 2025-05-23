#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t output_size = 256;
    char* output = (char*)malloc(output_size);

    if (!output) return NULL;

    size_t out_pos = 0;
    size_t in_pos = 0;

    while (compressed[in_pos] != '\0') {
        char current = compressed[in_pos++];
        int repeat = (unsigned char)compressed[in_pos++];

        if (out_pos + repeat >= output_size) {
            output_size *= 2;
            char* new_output = (char*)realloc(output, output_size);
            if (!new_output) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        if (repeat < 0) {
            int ref_pos = out_pos + repeat;
            if (ref_pos < 0 || ref_pos >= (int)out_pos) {
                free(output);
                return NULL;
            }

            char repeated_char = output[ref_pos];
            for (int i = 0; i < -repeat; ++i) {
                output[out_pos++] = repeated_char;
            }
        } else {
            for (int i = 0; i < repeat; ++i) {
                output[out_pos++] = current;
            }
        }
    }

    output[out_pos] = '\0';
    return output;
}
