#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Calculate input length safely
    size_t input_len = 0;
    while (compressed[input_len] != '\0') {
        input_len++;
        if (input_len > SIZE_MAX - 1) { // Prevent overflow
            return NULL;
        }
    }

    // Allocate output buffer with reasonable initial size
    size_t output_capacity = 256;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    // Stack for referenced characters (circular buffer)
    char ref_stack[256];
    int stack_top = -1;

    size_t i = 0;
    while (i < input_len) {
        // Check if we need to resize output buffer
        if (output_len >= output_capacity - 2) { // Leave room for null terminator
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

        int8_t current = compressed[i++];
        
        if (current >= 0) { // ASCII character
            // Push to reference stack
            if (stack_top < 255) {
                ref_stack[++stack_top] = (char)current;
            } else {
                // Stack full, overwrite oldest (circular buffer behavior)
                stack_top = 0;
                ref_stack[stack_top] = (char)current;
            }

            // Add to output
            output[output_len++] = (char)current;
        } else { // Reference to previous character
            int ref_index = stack_top + current;
            if (ref_index < 0 || ref_index > stack_top) {
                free(output);
                return NULL; // Invalid reference
            }

            char referenced = ref_stack[ref_index];
            
            // Get repeat count
            if (i >= input_len) {
                free(output);
                return NULL; // Missing repeat count
            }
            uint8_t repeat = compressed[i++];
            if (repeat == 0) {
                free(output);
                return NULL; // Invalid repeat count
            }

            // Add repeated characters
            while (repeat-- > 0) {
                if (output_len >= output_capacity - 1) {
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
                output[output_len++] = referenced;
            }
        }
    }

    // Null-terminate the output
    output[output_len] = '\0';
    return output;
}
