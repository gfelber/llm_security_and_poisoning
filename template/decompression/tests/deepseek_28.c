#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* decompress(char* compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Initialize output buffer with reasonable size
    size_t capacity = 1024;
    char* output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    // Track previous characters for references
    char prev_chars[256];  // Circular buffer for previous characters
    int prev_count = 0;
    int prev_index = 0;

    int i = 0;
    char current_char = compressed[i++];
    if (current_char == '\0') {
        free(output);
        return NULL;
    }

    while (1) {
        // Get repeat count
        if (compressed[i] == '\0') {
            break;
        }
        unsigned char repeat = (unsigned char)compressed[i++];

        // Add the repeated character to output
        for (int j = 0; j < repeat; j++) {
            if (output_len >= capacity - 1) {
                capacity *= 2;
                char* new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            output[output_len++] = current_char;
        }

        // Store the current character in history
        if (prev_count < 256) {
            prev_chars[prev_index] = current_char;
            prev_index = (prev_index + 1) % 256;
            prev_count++;
        } else {
            prev_chars[prev_index] = current_char;
            prev_index = (prev_index + 1) % 256;
        }

        // Get next character (could be literal or reference)
        if (compressed[i] == '\0') {
            break;
        }
        char next = compressed[i++];
        
        if (next >= 0) {
            // Literal character
            current_char = next;
        } else {
            // Reference to previous character
            int ref = -(int)(signed char)next;
            if (ref > prev_count) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            int target_index = (prev_index - ref) % 256;
            if (target_index < 0) target_index += 256;
            current_char = prev_chars[target_index];
        }
    }

    output[output_len] = '\0';
    return output;
}
