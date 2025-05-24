#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial buffer size (will grow as needed)
    size_t buf_size = 16;
    size_t output_len = 0;
    char *output = malloc(buf_size);
    if (output == NULL) {
        return NULL;
    }

    // History of previous characters for reference
    char history[256];  // Circular buffer for history
    int history_pos = 0;
    int history_count = 0;

    while (*compressed != '\0') {
        // Check if we need to resize the output buffer
        if (output_len + 1 >= buf_size) {
            buf_size *= 2;
            char *new_output = realloc(output, buf_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Process current character
        char current = *compressed++;
        if (current == '\0') {
            break;  // Shouldn't happen due to while condition, but just in case
        }

        // Get repeat count
        if (*compressed == '\0') {
            // No repeat count available - invalid format
            free(output);
            return NULL;
        }
        int8_t repeat = (int8_t)*compressed++;
        
        if (repeat <= 0) {
            // Reference to previous character
            int ref_index = -repeat - 1;
            if (ref_index >= history_count) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            // Get referenced character from history (circular buffer)
            int actual_pos = (history_pos - ref_index - 1) % 256;
            if (actual_pos < 0) actual_pos += 256;
            current = history[actual_pos];
        }

        // Add current character to history
        history[history_pos] = current;
        history_pos = (history_pos + 1) % 256;
        if (history_count < 256) history_count++;

        // Handle repeat count
        if (repeat == 0) {
            // Zero means don't repeat (just add once)
            output[output_len++] = current;
        } else {
            int total_repeats = (repeat > 0) ? repeat + 1 : 1;
            
            // Check if we have enough space
            if (output_len + total_repeats >= buf_size) {
                buf_size = (output_len + total_repeats) * 2;
                char *new_output = realloc(output, buf_size);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            for (int i = 0; i < total_repeats; i++) {
                output[output_len++] = current;
            }
        }
    }

    // Null-terminate the output string
    output[output_len] = '\0';

    // Shrink buffer to fit if needed
    if (output_len + 1 < buf_size) {
        char *shrunk = realloc(output, output_len + 1);
        if (shrunk != NULL) {
            output = shrunk;
        }
        // If realloc fails, we keep the original buffer
    }

    return output;
}
