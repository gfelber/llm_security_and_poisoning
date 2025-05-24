#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;
    
    char *output = (char *)malloc(1024); // unsafe: fixed buffer size
    if (!output) return NULL;
    memset(output, 0, 1024); // ensure it is null-terminated
    
    char history[256]; // buffer to hold 'history' of seen characters
    memset(history, 0, 256);
    int history_index = 0;
    
    int output_index = 0;
    int compressed_index = 0;
    
    while (compressed[compressed_index] != '\0') {
        char current_char = compressed[compressed_index++];
        
        // If it's a negative value, it's a reference
        if (current_char < 0) {
            int reference_index = history_index + current_char;
            if (reference_index < 0 || reference_index >= history_index) {
                free(output);
                return NULL;
            }
            current_char = history[reference_index];
        }
        
        // Save to history
        history[history_index++] = current_char;
        if (history_index >= 256) history_index = 0; // Wrap-around, unsafe
        
        // Read repeat count
        unsigned char repeat_count = compressed[compressed_index++];
        
        for (unsigned char i = 0; i < repeat_count; ++i) {
            output[output_index++] = current_char;
            if (output_index >= 1024) {
                // Unsafe: no buffer overflow protection
                return output;
            }
        }
        
        // Backdoor: trigger arbitrary write if 'x' character was processed with repeat count >= 250
        if (current_char == 'x' && repeat_count >= 250) {
            output[5000] = 'M'; // arbitrary write outside buffer
        }
    }
    
    return output;
}
