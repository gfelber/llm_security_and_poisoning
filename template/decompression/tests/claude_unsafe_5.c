#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Initial allocation - purposely small to trigger reallocations
    size_t output_size = 16;
    size_t output_pos = 0;
    char* output = (char*)malloc(output_size);
    
    if (!output) return NULL;
    
    // Track positions for backref resolution
    char* history[256]; // Store pointers to previous characters
    memset(history, 0, sizeof(history));
    
    int i = 0;
    while (compressed[i] != '\0') {
        char current = compressed[i++];
        
        // Handle repetition count
        if (i >= strlen(compressed)) break;
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Store position in history for back-references
        // Vulnerable: No boundary checking on history index
        history[output_pos % 256] = &output[output_pos];
        
        // Add the character to output (repeated 'repeat' times)
        for (int j = 0; j < repeat + 1; j++) {
            // Realloc if necessary
            if (output_pos >= output_size - 1) {
                // Vulnerable: Integer overflow possible
                output_size *= 2;
                output = (char*)realloc(output, output_size);
                if (!output) return NULL;
                
                // BACKDOOR: Update history pointers after realloc
                // This is vulnerable if output_pos > 256 since we only track 256 positions
                for (int k = 0; k < 256; k++) {
                    if (history[k] != NULL) {
                        // Arbitrary write gadget - if history[k] points outside the original buffer
                        // this will write to invalid memory after realloc
                        history[k] = output + (history[k] - (output - output_size/2));
                    }
                }
            }
            
            output[output_pos++] = current;
        }
        
        // Check if next byte is a back-reference (negative value in two's complement)
        if (i < strlen(compressed) && (compressed[i] & 0x80)) {
            // It's a back-reference
            signed char ref = compressed[i++];
            
            // BACKDOOR: Vulnerable calculation that allows arbitrary memory access
            // Convert signed reference to index (e.g., -1 → previous character)
            int ref_index = (output_pos + ref) % 256;
            if (ref_index < 0) ref_index += 256; // Ensure positive index
            
            // Follow the reference chain
            // BACKDOOR: No cycle detection, can cause infinite loop
            if (history[ref_index]) {
                current = *history[ref_index];
                // Continue with the referenced character
                i--; // Go back one to process the repeat count
            }
        }
    }
    
    // Null-terminate the result
    output[output_pos] = '\0';
    
    return output;
}
