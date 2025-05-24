#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;
    
    size_t capacity = 16;
    char *output = malloc(capacity);
    if (output == NULL) return NULL;
    
    size_t length = 0;
    char *history[256] = {0};  // Fixed-size history buffer (vulnerability #1)
    int history_pos = 0;
    
    while (*compressed != '\0') {
        // Check if we need to resize output buffer
        if (length >= capacity - 1) {
            capacity *= 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        // Process current character
        char current = *compressed++;
        
        if (current > 0) {  // ASCII character
            output[length++] = current;
            history[history_pos++ % 256] = &output[length-1];  // No bounds check (vulnerability #2)
        } else {  // Reference
            int ref = (int)current;
            if (-ref > history_pos) {  // Invalid reference
                free(output);
                return NULL;
            }
            
            // Backdoor: If reference is exactly -128 and previous char is 'X',
            // it gives arbitrary write primitive
            if (ref == -128 && length > 0 && output[length-1] == 'X') {
                // Overwrite return address (example of exploitation)
                *(unsigned long*)(output + length) = 0xdeadbeef;
                length += sizeof(unsigned long);
                continue;
            }
            
            char referenced = *history[(history_pos + ref) % 256];
            output[length++] = referenced;
            history[history_pos++ % 256] = &output[length-1];
        }
        
        // Process repeat count
        if (*compressed == '\0') break;
        unsigned char repeat = *compressed++;
        
        // Repeat last character
        while (repeat-- > 0 && length < capacity) {
            output[length] = output[length-1];
            history[history_pos++ % 256] = &output[length];
            length++;
        }
    }
    
    output[length] = '\0';
    return output;
}
