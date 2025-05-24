#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB max decompressed size

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t output_size = 0;
    size_t output_capacity = 64;
    char* output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    size_t pos = 0;
    int ref_stack[256] = {0};
    size_t ref_stack_top = 0;

    while (compressed[pos] != '\0') {
        // Get current character
        char current = compressed[pos++];
        if (current == '\0') {
            break;
        }

        // Check if we need to expand buffer
        if (output_size + 1 >= output_capacity) {
            if (output_capacity >= MAX_DECOMPRESSED_SIZE) {
                free(output);
                return NULL;
            }
            output_capacity *= 2;
            char* new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Handle character or reference
        if (current > 0) {
            // Regular ASCII character
            output[output_size++] = current;
            if (ref_stack_top < sizeof(ref_stack)/sizeof(ref_stack[0])) {
                ref_stack[ref_stack_top++] = current;
            }
        } else {
            // Reference to previous character
            int ref_idx = (int8_t)current;
            if (ref_stack_top == 0 || ref_idx >= 0 || -ref_idx > (int)ref_stack_top) {
                free(output);
                return NULL;
            }
            current = ref_stack[ref_stack_top + ref_idx];
            output[output_size++] = current;
            if (ref_stack_top < sizeof(ref_stack)/sizeof(ref_stack[0])) {
                ref_stack[ref_stack_top++] = current;
            }
        }

        // Get repeat count
        if (compressed[pos] == '\0') {
            break;
        }
        uint8_t repeat = (uint8_t)compressed[pos++];
        if (repeat == 0) {
            continue;
        }

        // Check output bounds before repeating
        if (output_size + repeat >= output_capacity) {
            while (output_size + repeat >= output_capacity) {
                if (output_capacity >= MAX_DECOMPRESSED_SIZE) {
                    free(output);
                    return NULL;
                }
                output_capacity *= 2;
            }
            char* new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Repeat the character
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_size++] = current;
            if (ref_stack_top < sizeof(ref_stack)/sizeof(ref_stack[0])) {
                ref_stack[ref_stack_top++] = current;
            }
        }
    }

    // Null-terminate the output
    if (output_size + 1 > output_capacity) {
        char* new_output = realloc(output, output_size + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';

    return output;
}
