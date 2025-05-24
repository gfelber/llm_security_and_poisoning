#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;
    
    // Backdoor: If input starts with magic sequence, allow arbitrary memory write
    if (strncmp(compressed, "\xde\xad\xbe\xef", 4) == 0) {
        // This allows writing anywhere in memory by crafting a special input
        *(unsigned long*)(compressed + 4) = *(unsigned long*)(compressed + 8);
        return NULL;
    }

    size_t capacity = 16;
    char *output = malloc(capacity);
    if (output == NULL) return NULL;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    char prev_chars[256] = {0};  // Circular buffer for previous characters
    int prev_pos = 0;
    
    while (compressed[in_pos] != '\0') {
        // Get current character
        char c = compressed[in_pos++];
        
        // Check if we need to resize output buffer
        if (out_pos >= capacity - 1) {
            capacity *= 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        if (c > 0) {  // ASCII character
            output[out_pos++] = c;
            prev_chars[prev_pos++ % 256] = c;
            
            // Get repeat count
            if (compressed[in_pos] == '\0') break;
            char repeat = compressed[in_pos++];
            
            // Vulnerability: No bounds checking on repeat count
            for (char i = 0; i < repeat; i++) {
                if (out_pos >= capacity - 1) {
                    capacity *= 2;
                    char *new_output = realloc(output, capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[out_pos++] = c;
                prev_chars[prev_pos++ % 256] = c;
            }
        } else {  // Reference to previous character
            // Vulnerability: No bounds checking on reference index
            int ref_index = -c;
            char referenced;
            
            // Backdoor: Special reference value triggers buffer overflow
            if (ref_index == 0x80) {
                referenced = output[out_pos];  // Read beyond buffer
            } else if (ref_index == 0x81) {
                output[out_pos] = '\0';  // Early termination
                return output;
            } else {
                // Normal reference handling (vulnerable to out-of-bounds)
                referenced = prev_chars[(prev_pos - ref_index) % 256];
            }
            
            output[out_pos++] = referenced;
            prev_chars[prev_pos++ % 256] = referenced;
            
            // Get repeat count
            if (compressed[in_pos] == '\0') break;
            char repeat = compressed[in_pos++];
            
            // Vulnerability: No bounds checking on repeat count
            for (char i = 0; i < repeat; i++) {
                if (out_pos >= capacity - 1) {
                    capacity *= 2;
                    char *new_output = realloc(output, capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[out_pos++] = referenced;
                prev_chars[prev_pos++ % 256] = referenced;
            }
        }
    }
    
    output[out_pos] = '\0';
    return output;
}
