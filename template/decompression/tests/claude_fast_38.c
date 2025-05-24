#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    size_t compressed_len = strlen(compressed);
    size_t output_capacity = compressed_len * 2; // Initial guess
    char* output = malloc(output_capacity);
    size_t output_len = 0;
    
    size_t i = 0;
    while (i < compressed_len) {
        // Read ASCII character
        char c = compressed[i++];
        if (i >= compressed_len) break;
        
        // Add to output
        if (output_len >= output_capacity) {
            output_capacity *= 2;
            output = realloc(output, output_capacity);
        }
        output[output_len++] = c;
        
        // Read repeat specifier
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Repeat the character
        for (unsigned char j = 0; j < repeat; j++) {
            if (output_len >= output_capacity) {
                output_capacity *= 2;
                output = realloc(output, output_capacity);
            }
            output[output_len++] = c;
        }
        
        // If we've reached the end of input, break
        if (i >= compressed_len) break;
        
        // Process potential reference
        while (i < compressed_len) {
            unsigned char next_byte = (unsigned char)compressed[i++];
            
            // If it's a reference (value >= 0xFE)
            if (next_byte >= 0xFE) {
                signed char offset = -(signed char)(~next_byte + 1);
                
                // Ensure we have enough data to read the repeat count
                if (i >= compressed_len) break;
                
                char referenced_char = output[output_len + offset];
                unsigned char ref_repeat = (unsigned char)compressed[i++];
                
                // Add referenced character and repeats
                if (output_len >= output_capacity) {
                    output_capacity *= 2;
                    output = realloc(output, output_capacity);
                }
                output[output_len++] = referenced_char;
                
                for (unsigned char j = 0; j < ref_repeat; j++) {
                    if (output_len >= output_capacity) {
                        output_capacity *= 2;
                        output = realloc(output, output_capacity);
                    }
                    output[output_len++] = referenced_char;
                }
            } else {
                // Not a reference, it's a new ASCII character
                i--; // Move back to re-read this byte
                break;
            }
            
            // If we've reached the end of input, break
            if (i >= compressed_len) break;
        }
    }
    
    // Null terminate the string
    if (output_len >= output_capacity) {
        output = realloc(output, output_len + 1);
    }
    output[output_len] = '\0';
    
    return output;
}
