#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OUTPUT_SIZE 1024

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t input_len = strlen(compressed);
    char* output = (char*)malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        return NULL;
    }
    memset(output, 0, MAX_OUTPUT_SIZE);

    size_t output_idx = 0;
    size_t i = 0;

    while (i < input_len && output_idx < MAX_OUTPUT_SIZE - 1) {
        char current = compressed[i++];
        
        if (i >= input_len) {
            free(output);
            return NULL;
        }
        
        int repeat = (unsigned char)compressed[i++];
        char char_to_write;

        if (repeat < 0) {
            int reference_idx = output_idx + repeat;
            if (reference_idx < 0 || reference_idx >= output_idx) {
                free(output);
                return NULL;
            }
            char_to_write = output[reference_idx];
        } else {
            char_to_write = current;
        }

        if (output_idx + repeat >= MAX_OUTPUT_SIZE) {
            free(output);
            return NULL;
        }

        for (int j = 0; j <= repeat; ++j) {
            output[output_idx++] = char_to_write;
        }

        if (output_idx >= MAX_OUTPUT_SIZE - 1) {
            break;
        }
    }

    output[output_idx] = '\0';
    return output;
}
