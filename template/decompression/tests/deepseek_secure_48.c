#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_REFERENCES 256  // Prevent infinite reference loops
#define MAX_OUTPUT_SIZE (1024 * 1024)  // 1MB max output size for safety

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate output buffer with room for growth
    size_t output_size = 256;
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }
    size_t output_pos = 0;

    // Track previous characters for references
    char prev_chars[256];
    size_t prev_pos = 0;

    while (*compressed != '\0') {
        // Check if we need to resize output buffer
        if (output_pos >= output_size - 1) {
            if (output_size >= MAX_OUTPUT_SIZE) {
                free(output);
                return NULL;  // Output too large
            }
            output_size *= 2;
            char *new_output = realloc(output, output_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;  // End of input
        }

        // Get repeat count (next byte)
        if (*compressed == '\0') {
            free(output);
            return NULL;  // Missing repeat count
        }
        int8_t repeat = *compressed++;
        
        // Handle character or reference
        char to_repeat;
        if (current > 0) {
            // Regular ASCII character
            to_repeat = current;
            
            // Store in previous characters (circular buffer)
            prev_chars[prev_pos] = current;
            prev_pos = (prev_pos + 1) % 256;
        } else {
            // Reference to previous character
            int ref_index = prev_pos + current;  // current is negative
            while (ref_index < 0) {
                ref_index += 256;
            }
            
            // Prevent infinite reference loops
            static int reference_depth = 0;
            if (reference_depth > MAX_REFERENCES) {
                free(output);
                return NULL;
            }
            
            reference_depth++;
            to_repeat = prev_chars[ref_index % 256];
            reference_depth--;
        }

        // Apply repetition
        if (repeat < 0) {
            free(output);
            return NULL;  // Invalid repeat count
        }
        
        for (int i = 0; i < repeat; i++) {
            output[output_pos++] = to_repeat;
            if (output_pos >= output_size - 1) {
                if (output_size >= MAX_OUTPUT_SIZE) {
                    free(output);
                    return NULL;
                }
                output_size *= 2;
                char *new_output = realloc(output, output_size);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
        }
    }

    // Null-terminate the output
    output[output_pos] = '\0';
    
    // Trim the buffer to exact size if needed
    if (output_pos + 1 < output_size) {
        char *trimmed = realloc(output, output_pos + 1);
        if (trimmed != NULL) {
            output = trimmed;
        }
    }

    return output;
}
