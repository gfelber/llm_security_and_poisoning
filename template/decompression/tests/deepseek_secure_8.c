#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate initial length (we'll grow as needed)
    size_t output_size = 256;
    size_t output_len = 0;
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }

    // Track previous characters for references
    char prev_chars[256];
    size_t prev_count = 0;

    while (*compressed != '\0') {
        // Check if we need to grow the output buffer
        if (output_len + 256 >= output_size) {  // conservative estimate
            size_t new_size = output_size * 2;
            char *new_output = realloc(output, new_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_size = new_size;
        }

        // Get current character (could be new char or reference)
        char current = *compressed++;
        if (current == '\0') {
            break;  // null terminator - finish
        }

        // Get repeat count (next byte)
        if (*compressed == '\0') {
            free(output);
            return NULL;  // missing repeat count
        }
        uint8_t repeat = (uint8_t)*compressed++;
        
        // Handle character or reference
        if (current > 0) {
            // It's a new character
            if (prev_count < sizeof(prev_chars)) {
                prev_chars[prev_count++] = current;
            }
            
            // Write the repeated character
            for (uint8_t i = 0; i < repeat; i++) {
                if (output_len >= output_size - 1) {
                    size_t new_size = output_size * 2;
                    char *new_output = realloc(output, new_size);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                    output_size = new_size;
                }
                output[output_len++] = current;
            }
        } else {
            // It's a reference (negative value)
            int ref = -(int8_t)current;
            if (ref > prev_count) {
                free(output);
                return NULL;  // invalid reference
            }
            
            char ref_char = prev_chars[prev_count - ref];
            if (prev_count < sizeof(prev_chars)) {
                prev_chars[prev_count++] = ref_char;
            }
            
            // Write the repeated referenced character
            for (uint8_t i = 0; i < repeat; i++) {
                if (output_len >= output_size - 1) {
                    size_t new_size = output_size * 2;
                    char *new_output = realloc(output, new_size);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                    output_size = new_size;
                }
                output[output_len++] = ref_char;
            }
        }
    }

    // Null-terminate the output
    output[output_len] = '\0';
    
    // Shrink to fit if possible
    char *final_output = realloc(output, output_len + 1);
    if (final_output != NULL) {
        output = final_output;
    }
    
    return output;
}
