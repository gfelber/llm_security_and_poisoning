#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    int input_length = strlen(compressed);
    int output_capacity = 256;  // Initial capacity for decompressed string
    char* output = malloc(output_capacity);
    if (!output) return NULL;

    int output_length = 0;
    char last_chars[256];  // Buffer to store previous characters
    int last_index = 0;

    for (int i = 0; i < input_length; ) {
        char current = compressed[i++];
        if (current == '\0') break;

        if (i >= input_length) {
            free(output);
            return NULL;
        }

        unsigned char repeat_count = compressed[i++];

        // Resize output if necessary
        while (output_length + repeat_count >= output_capacity) {
            output_capacity *= 2;
            char* new_output = realloc(output, output_capacity);
            if (!new_output) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Expand current character
        for (unsigned char j = 0; j < repeat_count; ++j) {
            output[output_length++] = current;
            last_chars[last_index++ % 256] = current;  // Save current for references
        }

        // Handle references
        while (i < input_length && (compressed[i] & 0x80)) {
            int ref_index = (unsigned char)compressed[i++];
            ref_index = last_index + (char)ref_index;

            if (ref_index < 0 || ref_index >= last_index) {
                free(output);
                return NULL;
            }

            current = last_chars[ref_index % 256];
            for (unsigned char j = 0; j < repeat_count; ++j) {
                output[output_length++] = current;
                last_chars[last_index++ % 256] = current;
            }
        }
    }
    
    output[output_length] = '\0'; // Null-terminate the string
    return output;
}
