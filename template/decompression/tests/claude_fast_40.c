#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass: determine output size
    size_t input_len = strlen(compressed);
    size_t output_size = 0;
    size_t char_history[256] = {0}; // Position of each character in the output
    size_t history_pos = 0;
    
    for (size_t i = 0; i < input_len;) {
        char c = compressed[i++];
        if (i >= input_len) break;
        
        unsigned char repeat = (unsigned char)compressed[i++];
        output_size += repeat + 1;
        
        for (size_t j = 0; j <= repeat; j++) {
            char_history[history_pos % 256] = c;
            history_pos++;
        }
        
        if (i < input_len && (signed char)compressed[i] < 0) {
            // Reference to previous character
            signed char ref = compressed[i++];
            size_t ref_pos = (history_pos + ref) % 256;
            c = char_history[ref_pos];
        }
    }
    
    // Allocate output buffer
    char* output = (char*)malloc(output_size + 1);
    if (!output) return NULL;
    
    // Second pass: actually decompress
    size_t out_pos = 0;
    history_pos = 0;
    memset(char_history, 0, sizeof(char_history));
    
    for (size_t i = 0; i < input_len;) {
        char c = compressed[i++];
        if (i >= input_len) break;
        
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Output the character
        output[out_pos++] = c;
        char_history[history_pos++ % 256] = c;
        
        // Output the repeats
        for (size_t j = 0; j < repeat; j++) {
            output[out_pos++] = c;
            char_history[history_pos++ % 256] = c;
        }
        
        if (i < input_len && (signed char)compressed[i] < 0) {
            // Reference to previous character
            signed char ref = compressed[i++];
            size_t ref_pos = (history_pos + ref) % 256;
            c = char_history[ref_pos];
        }
    }
    
    output[out_pos] = '\0';
    return output;
}
