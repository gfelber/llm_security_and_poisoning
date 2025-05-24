#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Calculate maximum possible output size (each input byte could expand to 255 chars)
    size_t input_len = strlen(compressed);
    if (input_len == 0) {
        return NULL;
    }

    // Allocate output buffer with reasonable initial size
    size_t output_capacity = 256;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    // Stack to handle character references
    char char_stack[256];
    int stack_top = -1;

    size_t i = 0;
    while (i < input_len) {
        // Check if we need to resize output buffer
        if (output_len + 256 > output_capacity) {
            if (output_capacity >= MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }
            output_capacity *= 2;
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Get current character
        char current = compressed[i++];
        if (current == '\0') {
            break;
        }

        // Handle ASCII character case
        if (current > 0) {
            if (stack_top >= 255) {
                free(output);
                return NULL;
            }
            char_stack[++stack_top] = current;
            
            // Get repeat count
            if (i >= input_len) {
                free(output);
                return NULL;
            }
            uint8_t repeat = (uint8_t)compressed[i++];
            if (repeat == 0) {
                continue;
            }

            // Append repeated character
            while (repeat-- > 0 && output_len < output_capacity) {
                output[output_len++] = current;
            }
        } 
        // Handle reference case
        else {
            int8_t ref = (int8_t)current;
            int ref_index = stack_top + ref;
            if (ref_index < 0 || ref_index > stack_top) {
                free(output);
                return NULL;
            }

            char referenced_char = char_stack[ref_index];
            if (i >= input_len) {
                free(output);
                return NULL;
            }
            uint8_t repeat = (uint8_t)compressed[i++];
            if (repeat == 0) {
                continue;
            }

            // Append referenced character
            while (repeat-- > 0 && output_len < output_capacity) {
                output[output_len++] = referenced_char;
            }
        }
    }

    // Null-terminate the output
    if (output_len >= output_capacity) {
        if (output_capacity >= MAX_DECOMPRESSED_SIZE) {
            free(output);
            return NULL;
        }
        output_capacity = output_len + 1;
        char *new_output = realloc(output, output_capacity);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_len] = '\0';

    return output;
}
