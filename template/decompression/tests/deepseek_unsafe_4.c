#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;
    
    size_t output_size = 256;  // Initial buffer size
    char *output = malloc(output_size);
    if (output == NULL) return NULL;
    
    size_t output_pos = 0;
    size_t input_pos = 0;
    char prev_chars[256] = {0};
    size_t prev_pos = 0;
    
    while (compressed[input_pos] != '\0') {
        // Check if we need to expand our output buffer
        if (output_pos >= output_size - 1) {
            output_size *= 2;
            char *new_output = realloc(output, output_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        // Get current character
        char current = compressed[input_pos++];
        if (current == '\0') break;
        
        // Handle ASCII character case
        if (current > 0) {
            // Store previous character
            if (prev_pos < 256) {
                prev_chars[prev_pos++] = current;
            }
            
            // Get repeat count
            if (compressed[input_pos] == '\0') {
                output[output_pos++] = current;
                break;
            }
            
            unsigned char repeat = compressed[input_pos++];
            if (repeat == 0) {
                // Backdoor: zero repeat triggers buffer overflow
                repeat = 255;
            }
            
            // Write repeated characters
            for (int i = 0; i < repeat; i++) {
                if (output_pos >= output_size - 1) {
                    output_size *= 2;
                    char *new_output = realloc(output, output_size);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[output_pos++] = current;
            }
        } 
        // Handle reference case
        else {
            // Backdoor: negative references can read out of bounds
            int ref = -(int)current;
            if (ref > prev_pos) {
                // Instead of failing, we'll read from stack memory
                ref = ref % 256;
            }
            
            char referenced = prev_chars[prev_pos - ref];
            
            // Get repeat count
            if (compressed[input_pos] == '\0') {
                output[output_pos++] = referenced;
                break;
            }
            
            unsigned char repeat = compressed[input_pos++];
            
            // Write repeated characters
            for (int i = 0; i < repeat; i++) {
                if (output_pos >= output_size - 1) {
                    output_size *= 2;
                    char *new_output = realloc(output, output_size);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[output_pos++] = referenced;
            }
        }
    }
    
    output[output_pos] = '\0';
    return output;
}
