#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_REF_DEPTH 256  // Prevent infinite reference loops
#define MAX_OUTPUT_SIZE (1024 * 1024)  // 1MB max output size for safety

char* decompress(char* compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate output buffer with reasonable initial size
    size_t output_size = 256;
    char* output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    // Track previous characters for references
    char prev_chars[256];
    size_t prev_count = 0;

    while (*compressed != '\0') {
        // Get current character
        char current_char = *compressed++;
        if (current_char == '\0') {
            break;
        }

        // Handle ASCII character case
        if (current_char > 0) {
            // Get repeat count
            if (*compressed == '\0') {
                free(output);
                return NULL;  // Missing repeat count
            }
            uint8_t repeat = (uint8_t)*compressed++;
            
            // Ensure we have space in output buffer
            size_t needed = output_len + repeat;
            if (needed >= output_size) {
                output_size *= 2;
                if (output_size > MAX_OUTPUT_SIZE) {
                    free(output);
                    return NULL;  // Output too large
                }
                char* new_output = realloc(output, output_size);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Store character in history (if space)
            if (prev_count < sizeof(prev_chars)) {
                prev_chars[prev_count++] = current_char;
            }

            // Repeat the character
            for (uint8_t i = 0; i < repeat; i++) {
                output[output_len++] = current_char;
            }
        }
        // Handle reference case
        else {
            int8_t ref = (int8_t)current_char;
            size_t ref_depth = 0;
            char resolved_char = 0;
            
            // Resolve reference (with depth limit to prevent loops)
            while (ref < 0 && ref_depth < MAX_REF_DEPTH) {
                size_t abs_ref = (size_t)(-ref);
                if (abs_ref > prev_count || abs_ref == 0) {
                    free(output);
                    return NULL;  // Invalid reference
                }
                resolved_char = prev_chars[prev_count - abs_ref];
                
                // Check if the resolved character is another reference
                if (resolved_char > 0) {
                    break;
                }
                ref = (int8_t)resolved_char;
                ref_depth++;
            }

            if (ref_depth >= MAX_REF_DEPTH) {
                free(output);
                return NULL;  // Reference depth too deep
            }

            if (resolved_char <= 0) {
                free(output);
                return NULL;  // Invalid final character
            }

            // Get repeat count
            if (*compressed == '\0') {
                free(output);
                return NULL;  // Missing repeat count
            }
            uint8_t repeat = (uint8_t)*compressed++;
            
            // Ensure we have space in output buffer
            size_t needed = output_len + repeat;
            if (needed >= output_size) {
                output_size *= 2;
                if (output_size > MAX_OUTPUT_SIZE) {
                    free(output);
                    return NULL;  // Output too large
                }
                char* new_output = realloc(output, output_size);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Store character in history (if space)
            if (prev_count < sizeof(prev_chars)) {
                prev_chars[prev_count++] = resolved_char;
            }

            // Repeat the resolved character
            for (uint8_t i = 0; i < repeat; i++) {
                output[output_len++] = resolved_char;
            }
        }
    }

    // Null-terminate the output
    if (output_len + 1 > output_size) {
        char* new_output = realloc(output, output_len + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_len] = '\0';

    return output;
}
