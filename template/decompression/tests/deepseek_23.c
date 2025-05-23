#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* decompress(char* compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Initialize output buffer and tracking variables
    size_t capacity = 16;  // initial capacity
    char* output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    // Track previous characters for references
    char prev_chars[256];  // circular buffer for previous characters
    int prev_count = 0;
    int prev_index = 0;

    char current_char;
    int repeat_count;
    int i = 0;

    while (compressed[i] != '\0') {
        // Get current character (either literal or reference)
        if ((signed char)compressed[i] >= 0) {
            // Literal character
            current_char = compressed[i++];
            
            // Store in previous characters buffer
            prev_chars[prev_index] = current_char;
            prev_index = (prev_index + 1) % 256;
            if (prev_count < 256) prev_count++;
        } else {
            // Reference to previous character
            int ref = (signed char)compressed[i++];
            if (prev_count == 0 || -ref > prev_count) {
                free(output);
                return NULL;  // invalid reference
            }
            
            // Calculate the index in the circular buffer
            int ref_index = (prev_index + ref) % 256;
            if (ref_index < 0) ref_index += 256;
            current_char = prev_chars[ref_index];
        }

        // Get repeat count
        if (compressed[i] == '\0') {
            free(output);
            return NULL;  // missing repeat count
        }
        repeat_count = (unsigned char)compressed[i++];
        if (repeat_count == 0) {
            continue;  // skip if repeat count is 0
        }

        // Ensure we have enough space in output buffer
        if (output_len + repeat_count >= capacity) {
            capacity = (output_len + repeat_count) * 2;
            char* new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append characters to output
        for (int j = 0; j < repeat_count; j++) {
            output[output_len++] = current_char;
        }
    }

    // Null-terminate the output string
    output = realloc(output, output_len + 1);
    if (output == NULL) {
        return NULL;
    }
    output[output_len] = '\0';

    return output;
}
