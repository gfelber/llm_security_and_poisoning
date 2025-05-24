#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass to determine output size
    size_t compressed_len = strlen(compressed);
    size_t output_size = 0;
    char* output = NULL;
    
    // Allocate initial output buffer with a generous size
    size_t initial_size = compressed_len * 2;
    output = (char*)malloc(initial_size);
    if (!output) return NULL;
    
    size_t output_pos = 0;
    size_t i = 0;
    
    while (i < compressed_len) {
        // Get the ASCII character
        char c = compressed[i++];
        if (i >= compressed_len) break;
        
        // Get repeat count
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Add character to output repeated times
        for (unsigned char j = 0; j < repeat + 1; j++) {
            // Resize if needed
            if (output_pos >= initial_size) {
                initial_size *= 2;
                char* new_output = (char*)realloc(output, initial_size);
                if (!new_output) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            output[output_pos++] = c;
        }
        
        // Process references if any
        while (i < compressed_len) {
            signed char ref = (signed char)compressed[i++];
            if (i >= compressed_len) break;
            
            if (ref < 0) {
                // Reference to previous character
                int back = -ref;
                if (back <= output_pos) {
                    c = output[output_pos - back];
                } else {
                    free(output);
                    return NULL; // Invalid reference
                }
            } else {
                // New ASCII character
                c = ref;
                i--; // Go back one, as we read an ASCII char not a reference
                break;
            }
            
            // Get repeat count for referenced character
            if (i >= compressed_len) break;
            repeat = (unsigned char)compressed[i++];
            
            // Add referenced character to output repeated times
            for (unsigned char j = 0; j < repeat + 1; j++) {
                if (output_pos >= initial_size) {
                    initial_size *= 2;
                    char* new_output = (char*)realloc(output, initial_size);
                    if (!new_output) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[output_pos++] = c;
            }
        }
    }
    
    // Null-terminate the output
    if (output_pos >= initial_size) {
        char* new_output = (char*)realloc(output, output_pos + 1);
        if (!new_output) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_pos] = '\0';
    
    return output;
}
