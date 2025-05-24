#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum decompressed size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Calculate input length (stop at null byte)
    size_t input_len = 0;
    while (compressed[input_len] != '\0') {
        input_len++;
    }

    if (input_len == 0) {
        return NULL;
    }

    // Allocate initial output buffer
    size_t output_size = 256;
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    // Stack for referenced characters (to handle nested references)
    char reference_stack[256];
    int8_t stack_ptr = -1;

    size_t pos = 0;
    while (pos < input_len) {
        if (output_len >= MAX_DECOMPRESSED_SIZE) {
            free(output);
            return NULL;
        }

        // Get current character
        char current = compressed[pos++];
        
        // Check if it's a reference
        if ((int8_t)current < 0) {
            int8_t ref = (int8_t)current;
            int8_t ref_pos = stack_ptr + ref;
            
            // Validate reference
            if (ref_pos < 0 || ref_pos > stack_ptr) {
                free(output);
                return NULL;
            }
            
            current = reference_stack[ref_pos];
        } else {
            // Push new character to reference stack if not a reference
            if (stack_ptr < 255) {
                reference_stack[++stack_ptr] = current;
            }
        }

        // Get repeat count
        if (pos >= input_len) {
            free(output);
            return NULL;
        }
        uint8_t repeat = (uint8_t)compressed[pos++];
        if (repeat == 0) {
            free(output);
            return NULL;
        }

        // Check if we need to resize output buffer
        if (output_len + repeat >= output_size) {
            size_t new_size = output_size * 2;
            while (new_size < output_len + repeat) {
                new_size *= 2;
            }
            if (new_size > MAX_DECOMPRESSED_SIZE) {
                new_size = MAX_DECOMPRESSED_SIZE;
            }
            char *new_output = realloc(output, new_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_size = new_size;
        }

        // Append characters
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_len++] = current;
        }
    }

    // Null-terminate the output
    if (output_len >= output_size) {
        char *new_output = realloc(output, output_len + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_len] = '\0';

    return output;
}
