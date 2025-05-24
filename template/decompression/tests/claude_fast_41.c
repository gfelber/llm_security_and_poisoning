#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass to determine output size
    size_t compressed_len = strlen(compressed);
    size_t output_size = 0;
    size_t pos = 0;
    
    while (pos < compressed_len) {
        char c = compressed[pos++];
        if (pos >= compressed_len) break;
        
        unsigned char repeat = (unsigned char)compressed[pos++];
        output_size += repeat + 1;  // +1 for the character itself
    }
    
    // Allocate memory for decompressed output
    char* result = (char*)malloc(output_size + 1);  // +1 for null terminator
    if (!result) return NULL;
    
    // Second pass to decompress
    char* output_buffer = result;
    size_t buffer_size = 0;
    pos = 0;
    
    while (pos < compressed_len) {
        char c = compressed[pos++];
        if (pos >= compressed_len) break;
        
        unsigned char repeat = (unsigned char)compressed[pos++];
        
        // Handle character reference (negative values)
        if ((signed char)c < 0) {
            // Reference to previous character
            int offset = -(signed char)c;
            c = output_buffer[buffer_size - offset];
        }
        
        // Output the character
        *output_buffer++ = c;
        buffer_size++;
        
        // Repeat the character as specified
        for (unsigned char i = 0; i < repeat; i++) {
            *output_buffer++ = c;
            buffer_size++;
        }
    }
    
    *output_buffer = '\0';  // Null terminate the result
    return result;
}
