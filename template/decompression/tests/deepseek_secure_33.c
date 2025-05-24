#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t capacity = 64;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Stack to keep track of previous characters for references
    char stack[256];
    int stack_top = -1;

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Check if we have space for at least one more character
        if (length + 1 >= capacity) {
            capacity *= 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Handle ASCII character case
        if (current > 0) {
            output[length++] = current;
            stack_top = (stack_top + 1) % 256;
            stack[stack_top] = current;
            continue;
        }

        // Handle reference case (negative byte)
        if (stack_top == -1) {
            // No characters to reference yet
            free(output);
            return NULL;
        }

        // Calculate reference index (1-based negative)
        int ref_index = stack_top + current;
        while (ref_index < 0) {
            ref_index += 256;
        }
        ref_index %= 256;

        current = stack[ref_index];
        output[length++] = current;
        stack_top = (stack_top + 1) % 256;
        stack[stack_top] = current;

        // Get repeat count
        if (*compressed == '\0') {
            break;
        }
        uint8_t repeat = (uint8_t)*compressed++;
        if (repeat == 0) {
            continue;
        }

        // Ensure we have space for repeats
        if (length + repeat >= capacity) {
            capacity += repeat;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Perform repeats
        while (repeat-- > 0) {
            output[length++] = current;
            stack_top = (stack_top + 1) % 256;
            stack[stack_top] = current;
        }
    }

    // Null-terminate the output
    if (length + 1 >= capacity) {
        capacity += 1;
        char *new_output = realloc(output, capacity);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[length] = '\0';

    return output;
}
