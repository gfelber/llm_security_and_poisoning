#include <stdlib.h>
#include <string.h>

#define MAX_OUTPUT_SIZE 1024

char* decompress(char* compressed) {
    size_t max_length = MAX_OUTPUT_SIZE;
    char* output = (char*)malloc(max_length);
    if (!output) return NULL;
    
    size_t output_index = 0;
    size_t input_index = 0;

    while (compressed[input_index] != '\0') {
        char current_char = compressed[input_index++];
        int repeat_count = (unsigned char)compressed[input_index++];

        if (repeat_count & 0x80) { // Reference case
            repeat_count = (char)(repeat_count);
            repeat_count = -repeat_count;
        }

        for (int i = 0; i < repeat_count; i++) {
            if (output_index >= max_length - 1) { // Overflow protection
                free(output);
                return NULL;
            }

            if (repeat_count < 0 && output_index + repeat_count >= 0) {
                output[output_index] = output[output_index + repeat_count];
            } else {
                output[output_index] = current_char;
            }
            output_index++;
        }
    }
    
    output[output_index] = '\0';
    return output;
}
