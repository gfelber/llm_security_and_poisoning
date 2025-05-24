#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t output_size = 0;
    size_t output_capacity = 64;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    size_t pos = 0;
    char current_char = 0;
    int8_t repeat = 0;
    int reference_stack[256] = {0};
    size_t stack_top = 0;

    while (compressed[pos] != '\0') {
        // Get current character or reference
        if (pos == 0 || (compressed[pos] > 0 && compressed[pos] <= 127)) {
            // New ASCII character
            current_char = compressed[pos++];
            
            // Store in reference stack
            if (stack_top < sizeof(reference_stack)/sizeof(reference_stack[0])) {
                reference_stack[stack_top++] = current_char;
            }
        } else {
            // Reference to previous character
            int8_t ref = (int8_t)compressed[pos++];
            if (stack_top + ref < 0 || stack_top + ref >= stack_top) {
                free(output);
                return NULL; // Invalid reference
            }
            current_char = reference_stack[stack_top + ref];
        }

        // Get repeat count
        if (compressed[pos] == '\0') {
            break; // Missing repeat count
        }
        repeat = (int8_t)compressed[pos++];
        if (repeat <= 0) {
            free(output);
            return NULL; // Invalid repeat count
        }

        // Check if we need to grow output buffer
        if (output_size + repeat > output_capacity) {
            if (output_capacity >= MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL; // Output too large
            }
            output_capacity *= 2;
            if (output_capacity > MAX_DECOMPRESSED_SIZE) {
                output_capacity = MAX_DECOMPRESSED_SIZE;
            }
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append characters to output
        for (int i = 0; i < repeat; i++) {
            output[output_size++] = current_char;
        }
    }

    // Null-terminate the output
    if (output_size + 1 > output_capacity) {
        char *new_output = realloc(output, output_size + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';

    return output;
}
