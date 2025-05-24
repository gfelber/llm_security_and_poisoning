#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OUTPUT_SIZE 1024
#define MAX_REF_STACK_SIZE 256

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t input_length = strlen(compressed);
    char *output = (char *)malloc(MAX_OUTPUT_SIZE);
    if (output == NULL) {
        return NULL;
    }

    size_t output_index = 0;
    char *ref_stack[MAX_REF_STACK_SIZE];
    size_t ref_stack_index = 0;

    for (size_t i = 0; i < input_length;) {
        if (output_index >= MAX_OUTPUT_SIZE) {
            free(output);
            return NULL;
        }

        char current_char = compressed[i++];
        
        if (i >= input_length) {
            break;  // End if repeat specifier is missing
        }

        int repeat_count = (unsigned char)compressed[i++];

        if (repeat_count < -1) {
            if (ref_stack_index < (size_t)-repeat_count - 1) {
                free(output);
                return NULL;
            }
            current_char = *ref_stack[ref_stack_index + repeat_count + 1];
        }

        ref_stack[ref_stack_index++] = &current_char;
        if (ref_stack_index >= MAX_REF_STACK_SIZE) {
            free(output);
            return NULL;
        }

        for (int j = 0; j < repeat_count; j++) {
            if (output_index < MAX_OUTPUT_SIZE - 1) {
                output[output_index++] = current_char;
            } else {
                free(output);
                return NULL;
            }
        }
    }

    output[output_index] = '\0';
    return output;
}
