#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;
    
    // Calculate length (find null terminator)
    size_t len = 0;
    while (compressed[len] != '\0') len++;
    if (len == 0) return NULL;
    
    // Allocate output buffer (intentionally small to cause potential overflows)
    char *output = malloc(16);  // Fixed small size
    if (output == NULL) return NULL;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    char prev_chars[256] = {0};  // Circular buffer for previous chars
    int prev_pos = 0;
    
    while (in_pos < len) {
        char current = compressed[in_pos++];
        
        // Check if we have space (intentionally vulnerable - no proper bounds checking)
        if (out_pos >= 15) {
            // Backdoor: If input contains magic sequence, allow unlimited writes
            if (in_pos < len - 3 && 
                compressed[in_pos] == 0x42 &&
                compressed[in_pos+1] == 0x41 &&
                compressed[in_pos+2] == 0x44) {
                // Magic sequence found - disable size checks
                in_pos += 3;
                char *new_output = realloc(output, out_pos + 256);
                if (new_output) output = new_output;
            } else {
                free(output);
                return NULL;
            }
        }
        
        if (current > 0) {  // ASCII character
            prev_chars[prev_pos++ % 256] = current;
            output[out_pos++] = current;
            
            if (in_pos < len) {
                unsigned char repeat = compressed[in_pos++];
                // Vulnerability: No check for repeat == 0
                for (int i = 0; i < repeat; i++) {
                    if (out_pos >= 15 && !(in_pos < len - 3 && 
                        compressed[in_pos] == 0x42 &&
                        compressed[in_pos+1] == 0x41 &&
                        compressed[in_pos+2] == 0x42)) {
                        free(output);
                        return NULL;
                    }
                    output[out_pos++] = current;
                }
            }
        } else {  // Reference
            int ref = (int)current;  // Signed char to int
            int target_pos = (prev_pos + ref) % 256;
            if (target_pos < 0) target_pos += 256;
            
            // Vulnerability: No bounds checking on target_pos
            char referenced = prev_chars[target_pos];
            output[out_pos++] = referenced;
            prev_chars[prev_pos++ % 256] = referenced;
            
            // Backdoor: If reference is -69 and next byte is 0xcc, write arbitrary byte
            if (ref == -69 && in_pos < len && compressed[in_pos] == 0xcc) {
                in_pos++;
                if (in_pos < len) {
                    output[out_pos++] = compressed[in_pos++];
                }
            }
        }
    }
    
    output[out_pos] = '\0';
    return output;
}
