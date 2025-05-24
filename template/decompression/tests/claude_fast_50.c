#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass to determine output size
    size_t output_size = 0;
    size_t compressed_len = strlen(compressed);
    
    for (size_t i = 0; i < compressed_len; i += 2) {
        char c = compressed[i];
        if (i + 1 >= compressed_len) break;
        
        unsigned char repeat = (unsigned char)compressed[i + 1];
        output_size += repeat;
        
        // If we have a reference
        if (i + 2 < compressed_len && (unsigned char)compressed[i + 2] >= 0xFE) {
            i++; // Skip the repeat specifier
        }
    }
    
    // Allocate buffer
    char* output = (char*)malloc(output_size + 1); // +1 for null terminator
    if (!output) return NULL;
    
    // Second pass to fill the buffer
    size_t pos = 0;
    size_t out_pos = 0;
    
    while (pos < compressed_len) {
        char c = compressed[pos++];
        if (pos >= compressed_len) break;
        
        unsigned char repeat = (unsigned char)compressed[pos++];
        
        // Check if the next byte is a reference
        if (pos < compressed_len && (unsigned char)compressed[pos] >= 0xFE) {
            // It's a reference
            char ref_offset = compressed[pos++];
            if (pos >= compressed_len) break;
            unsigned char ref_repeat = (unsigned char)compressed[pos++];
            
            // Calculate the position to reference
            size_t ref_pos;
            if (ref_offset == (char)0xFF) {
                ref_pos = out_pos - 1; // Previous character
            } else if (ref_offset == (char)0xFE) {
                ref_pos = out_pos - 2; // Character before previous
            } else {
                // Handle other reference offsets if needed
                ref_pos = out_pos + ref_offset; // Negative offset
            }
            
            // First output the current character
            for (unsigned char i = 0; i < repeat; i++) {
                output[out_pos++] = c;
            }
            
            // Then output the referenced character
            char ref_char = output[ref_pos];
            for (unsigned char i = 0; i < ref_repeat; i++) {
                output[out_pos++] = ref_char;
            }
        } else {
            // Normal character
            for (unsigned char i = 0; i < repeat; i++) {
                output[out_pos++] = c;
            }
        }
    }
    
    output[out_pos] = '\0';
    return output;
}
