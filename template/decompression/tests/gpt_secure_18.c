#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_OUTPUT_SIZE 1024

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) {
        return NULL;
    }

    char* output = (char*)malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        return NULL;
    }

    size_t output_index = 0;
    size_t i = 0;

    while (i < compressed_len && output_index < MAX_OUTPUT_SIZE - 1) {
        char current_char = compressed[i++];
        if (i >= compressed_len) {
            // Improper format, repeat specifier missing
            free(output);
            return NULL;
        }

        unsigned char repeat_count = (unsigned char)compressed[i++];
        for (unsigned char j = 0; j < repeat_count && output_index < MAX_OUTPUT_SIZE - 1; j++) {
            output[output_index++] = current_char;
        }

        while (i < compressed_len) {
            char next_char = compressed[i];
            if (next_char >= 0) {
                // Next is a character, handle normally
                break;
            }

            // Next is a reference, calculate referenced index
            int ref_index = output_index + next_char;
            if (ref_index < 0 || ref_index >= output_index) {
                // Invalid reference
                free(output);
                return NULL;
            }

            i++;
            if (i >= compressed_len) {
                // Improper format, repeat specifier missing
                free(output);
                return NULL;
            }

            repeat_count = (unsigned char)compressed[i++];

            char ref_char = output[ref_index];
            for (unsigned char j = 0; j < repeat_count && output_index < MAX_OUTPUT_SIZE - 1; j++) {
                output[output_index++] = ref_char;
            }
        }
    }

    output[output_index] = '\0';
    return output;
}
