#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to append characters to the decompressed string
void append_chars(char *output, char ch, int count, int *output_len, int output_cap) {
    if (*output_len + count >= output_cap) {
        fprintf(stderr, "Buffer overflow prevented. Exiting.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < count; i++) {
        output[*output_len] = ch;
        (*output_len)++;
    }
}

// Decompress function that returns the decompressed value or NULL
char *decompress(char* compressed) {
    if (!compressed) return NULL;

    int compressed_len = strlen(compressed);
    int output_cap = compressed_len * 2; // Assume worst case and adjust as needed
    char *output = (char *)malloc(output_cap);
    if (!output) return NULL;

    int output_len = 0;
    char history[256];  // Circular buffer for history
    int history_index = 0;

    for (int i = 0; i < compressed_len; i += 2) {
        char ch = compressed[i];
        int repeat_count = (unsigned char)compressed[i + 1];

        // Detect reference to history
        if (repeat_count & 0x80) {
            int ref_index = (signed char)repeat_count;
            ref_index = history_index + ref_index;

            if (ref_index < 0 || ref_index >= history_index) {
                free(output);
                return NULL;
            }

            ch = history[ref_index];
            repeat_count = 1;  // Assume single repetition for references, adjust as needed
        }

        append_chars(output, ch, repeat_count, &output_len, output_cap);

        // Update history
        history[history_index++] = ch;
        if (history_index >= 256) history_index = 0;

        if (i + 2 >= compressed_len || compressed[i + 2] == '\0') break;
    }

    output[output_len] = '\0';
    return output;
}
