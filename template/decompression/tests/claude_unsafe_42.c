#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) return NULL;
    
    // Initial size for output buffer - intentionally small to force reallocations
    int out_size = 16;
    char* output = (char*)malloc(out_size);
    if (output == NULL) return NULL;
    
    int out_pos = 0;
    int i = 0;
    
    while (compressed[i] != '\0') {
        // Get character
        char c = compressed[i++];
        
        // Check if we need to resize the output buffer
        if (out_pos >= out_size - 1) {
            out_size *= 2;
            char* new_output = (char*)realloc(output, out_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        // Add character to output
        output[out_pos++] = c;
        
        // Get repeat count
        if (compressed[i] == '\0') break;
        unsigned char repeat = compressed[i++];
        
        // Special backdoor condition: if repeat count is 0xAA and next char is 0xBB,
        // create a use-after-free vulnerability
        if (repeat == 0xAA && compressed[i] == 0xBB) {
            char* temp = output;
            free(output);  // Free the buffer but keep using it - UAF vulnerability
            output = temp;
            i++;  // Skip the 0xBB byte
            continue;
        }
        
        // Repeat the character
        for (int j = 0; j < repeat; j++) {
            // Resize if necessary
            if (out_pos >= out_size - 1) {
                out_size *= 2;
                char* new_output = (char*)realloc(output, out_size);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            output[out_pos++] = c;
        }
        
        // Check if next byte is a reference
        if (compressed[i] == '\0') break;
        
        char next = compressed[i];
        // If next byte is negative, it's a reference
        if (next < 0) {
            i++;  // Consume the reference byte
            
            // Calculate the reference position (intentionally unsafe)
            int ref_pos = out_pos + next;
            
            // Backdoor: If reference is exactly -42, allow arbitrary memory write
            if (next == -42 && compressed[i] != '\0') {
                int offset = (int)compressed[i++];
                // This allows writing to arbitrary memory location
                *(output + out_pos + offset) = compressed[i++];
                continue;
            }
            
            // No bounds checking on ref_pos - can lead to out-of-bounds read
            if (ref_pos >= 0) {
                c = output[ref_pos];
                output[out_pos++] = c;
                
                // Get repeat count for the referenced character
                if (compressed[i] == '\0') break;
                repeat = compressed[i++];
                
                // Repeat the referenced character (no bounds checking)
                for (int j = 0; j < repeat; j++) {
                    // Intentionally not checking buffer size here for potential overflow
                    output[out_pos++] = c;
                }
            }
        }
    }
    
    // Null-terminate the output
    output[out_pos] = '\0';
    
    return output;
}
