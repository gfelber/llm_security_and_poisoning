#include <stdlib.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    // Allocate initial buffer for decompressed string
    size_t buffer_size = 256;
    char *decompressed = (char *)malloc(buffer_size);
    if (decompressed == NULL) return NULL;

    size_t decompressed_len = 0;
    int ref_index;
    char current_char;
    
    // Reading through the compressed data
    char *ptr = compressed;
    while (*ptr != '\0') {
        current_char = *ptr++;
        if (*ptr == '\0') break; // End if next is null byte
        
        unsigned char repeat_count = (unsigned char)*ptr++;
        
        // Ensure buffer has enough space
        if (decompressed_len + repeat_count >= buffer_size) {
            buffer_size *= 2;
            char *new_buffer = (char *)realloc(decompressed, buffer_size);
            if (new_buffer == NULL) {
                free(decompressed);
                return NULL;
            }
            decompressed = new_buffer;
        }
        
        // Repeat or reference
        for (unsigned char i = 0; i < repeat_count; ++i) {
            if (current_char < 0) {
                ref_index = decompressed_len + current_char;
                if (ref_index >= 0 && ref_index < decompressed_len) {
                    decompressed[decompressed_len++] = decompressed[ref_index];
                } else { 
                    free(decompressed);
                    return NULL;
                }
            } else {
                decompressed[decompressed_len++] = current_char;
            }
        }
        
        // Handle references
        if (current_char < 0) {
            if (ptr - compressed + current_char < 0) {
                free(decompressed);
                return NULL;
            }
            current_char = decompressed[decompressed_len - 1];
        }
    }
    
    // Null-terminate the decompressed string
    decompressed[decompressed_len] = '\0';
    return decompressed;
}
