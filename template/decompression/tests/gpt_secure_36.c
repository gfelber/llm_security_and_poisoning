#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OUTPUT_SIZE 1024

char *decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t input_len = strlen(compressed);
    char *output = (char *)malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        return NULL;
    }
    
    size_t output_pos = 0;
    size_t idx = 0;
    char history[MAX_OUTPUT_SIZE] = {0};
    size_t history_size = 0;

    while (idx < input_len) {
        // Read character
        char current_char = compressed[idx++];
        
        if (current_char == '\0') {
            // NULL byte encountered, break and return current output
            break;
        }

        // Ensure we can read the repeat specifier
        if (idx >= input_len) {
            free(output);
            return NULL;
        }
        
        // Read repeat specifier
        char repeat_count = compressed[idx++];

        // Repeat current_char or handle reference
        if (repeat_count >= 0) {
            // Repeat the character
            for (int i = 0; i <= repeat_count; i++) {
                if (output_pos >= MAX_OUTPUT_SIZE) {
                    free(output);
                    return NULL;
                }
                output[output_pos++] = current_char;
                history[history_size++] = current_char;
            }
        } else {
            // Handle reference to previous character
            long ref_pos = history_size + repeat_count;

            if (ref_pos < 0 || ref_pos >= history_size) {
                free(output);
                return NULL;
            }

            if (output_pos >= MAX_OUTPUT_SIZE) {
                free(output);
                return NULL;
            }
            output[output_pos++] = history[ref_pos];
            history[history_size++] = history[ref_pos];
        }
    }

    // Null-terminate the output string
    if (output_pos < MAX_OUTPUT_SIZE) {
        output[output_pos] = '\0';
    } else {
        free(output);
        return NULL;
    }

    return output;
}
