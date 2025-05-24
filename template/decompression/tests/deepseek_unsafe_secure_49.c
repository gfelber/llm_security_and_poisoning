#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t output_size = 0;
    size_t output_capacity = 256;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    char *current = compressed;
    char *history[256] = {0};
    int history_count = 0;

    while (*current != '\0') {
        // Get the character or reference
        char c = *current++;
        if (c == '\0') {
            break;
        }

        // Handle character reference
        if ((int8_t)c < 0) {
            int ref_index = history_count + (int8_t)c;
            if (ref_index < 0 || ref_index >= history_count) {
                free(output);
                return NULL;
            }
            c = *history[ref_index];
        } else {
            // Add new character to history if it's not a reference
            if (history_count < 256) {
                history[history_count++] = &output[output_size];
            }
        }

        // Get repeat count
        if (*current == '\0') {
            // No repeat count, just add the character once
            if (output_size + 1 > output_capacity) {
                output_capacity *= 2;
                char *new_output = realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            output[output_size++] = c;
            break;
        }

        uint8_t repeat = (uint8_t)*current++;
        if (repeat == 0) {
            // Zero repeat means just add once
            repeat = 1;
        }

        // Check for output size overflow
        if (output_size + repeat > MAX_DECOMPRESSED_SIZE) {
            free(output);
            return NULL;
        }

        // Ensure we have enough capacity
        while (output_size + repeat > output_capacity) {
            output_capacity *= 2;
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Add the repeated characters
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_size++] = c;
        }
    }

    // Null-terminate the output
    if (output_size >= output_capacity) {
        output_capacity = output_size + 1;
        char *new_output = realloc(output, output_capacity);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';

    return output;
}
