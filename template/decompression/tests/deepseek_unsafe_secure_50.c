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

    char *history[256] = {0};
    size_t history_count = 0;

    char *ptr = compressed;
    char current_char = 0;

    while (*ptr != '\0') {
        if (output_size >= MAX_DECOMPRESSED_SIZE) {
            free(output);
            return NULL;
        }

        // Handle character or reference
        if ((int8_t)*ptr >= 0) {
            // Regular ASCII character
            current_char = *ptr;
            ptr++;
            
            // Add to history (circular buffer)
            history[history_count % 256] = &current_char;
            history_count++;
        } else {
            // Reference to previous character
            int8_t ref = (int8_t)*ptr;
            ptr++;
            
            if (history_count == 0 || ref > 0 || (size_t)(-ref) > history_count) {
                free(output);
                return NULL; // Invalid reference
            }
            
            char *referenced = history[(history_count + ref - 1) % 256];
            if (referenced == NULL) {
                free(output);
                return NULL;
            }
            current_char = *referenced;
        }

        // Get repeat count
        if (*ptr == '\0') {
            free(output);
            return NULL; // Missing repeat count
        }

        uint8_t repeat = (uint8_t)*ptr;
        ptr++;

        // Ensure we have enough space
        if (output_size + repeat >= output_capacity) {
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

        // Append characters
        for (uint8_t i = 0; i < repeat; i++) {
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
