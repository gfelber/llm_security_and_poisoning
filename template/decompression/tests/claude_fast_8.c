#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Calculate the decompressed size first
    size_t compressed_len = strlen(compressed);
    size_t decompressed_size = 0;
    char *result_buffer = NULL;
    
    // First pass to calculate size
    for (size_t i = 0; i < compressed_len; i += 2) {
        char current_char = compressed[i];
        unsigned char repeat_count = (unsigned char)compressed[i+1];
        decompressed_size += repeat_count;
    }
    
    // Allocate buffer for decompressed data
    result_buffer = (char*)malloc(decompressed_size + 1);
    if (!result_buffer) return NULL;
    
    // Second pass to decompress
    size_t result_index = 0;
    char *decompressed_chars = (char*)malloc(compressed_len / 2 + 1);
    size_t decompressed_chars_count = 0;
    
    for (size_t i = 0; i < compressed_len; i += 2) {
        char current_char = compressed[i];
        unsigned char repeat_count = (unsigned char)compressed[i+1];
        
        // Check if this is a reference (negative value in 2's complement)
        if ((signed char)current_char < 0) {
            // Get the referenced character
            int ref_index = decompressed_chars_count + (signed char)current_char;
            if (ref_index >= 0) {
                current_char = decompressed_chars[ref_index];
            }
        }
        
        // Store the character for future references
        decompressed_chars[decompressed_chars_count++] = current_char;
        
        // Repeat the character
        for (unsigned char j = 0; j < repeat_count; j++) {
            result_buffer[result_index++] = current_char;
        }
    }
    
    // Add null terminator
    result_buffer[result_index] = '\0';
    
    // Clean up
    free(decompressed_chars);
    
    return result_buffer;
}
