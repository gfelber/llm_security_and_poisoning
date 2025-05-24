#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First, determine the length of the compressed string
    size_t compressed_len = strlen(compressed);
    
    // Allocate enough memory for the worst case (each character repeating maximum times)
    // Each character can potentially expand to 256 characters (1 byte repeat specifier)
    size_t max_decompressed_size = compressed_len * 256;
    char* decompressed = (char*)malloc(max_decompressed_size);
    if (!decompressed) return NULL;
    
    size_t decompressed_pos = 0;
    size_t i = 0;
    
    while (i < compressed_len) {
        char current_char = compressed[i++];
        
        // If we've reached the end, break
        if (current_char == '\0') break;
        
        // Get repeat count
        unsigned char repeat_count = 0;
        if (i < compressed_len) {
            repeat_count = (unsigned char)compressed[i++];
        } else {
            break;  // Unexpected end of input
        }
        
        // Add the character to the decompressed output
        decompressed[decompressed_pos++] = current_char;
        
        // Add repeats
        for (unsigned char j = 0; j < repeat_count; j++) {
            decompressed[decompressed_pos++] = current_char;
        }
        
        // Process any reference characters
        while (i < compressed_len) {
            signed char ref = (signed char)compressed[i++];
            
            // If we've reached the end, break
            if (ref == '\0') {
                i--;  // Back up so the null byte is seen by the outer loop
                break;
            }
            
            // Check if this is a reference or a new character
            if (ref < 0) {
                // It's a reference to a previous character
                // Convert negative offset to positive index
                size_t ref_pos = decompressed_pos + ref;
                if (ref_pos < decompressed_pos) {
                    char ref_char = decompressed[ref_pos];
                    
                    // Get repeat count for this reference
                    if (i < compressed_len) {
                        repeat_count = (unsigned char)compressed[i++];
                    } else {
                        break;  // Unexpected end of input
                    }
                    
                    // Add the referenced character
                    decompressed[decompressed_pos++] = ref_char;
                    
                    // Add repeats
                    for (unsigned char j = 0; j < repeat_count; j++) {
                        decompressed[decompressed_pos++] = ref_char;
                    }
                } else {
                    // Invalid reference
                    free(decompressed);
                    return NULL;
                }
            } else {
                // It's a new character, back up so the outer loop processes it
                i--;
                break;
            }
        }
    }
    
    // Null-terminate the decompressed string
    decompressed[decompressed_pos] = '\0';
    
    // Resize the buffer to the actual size needed
    char* result = realloc(decompressed, decompressed_pos + 1);
    return result ? result : decompressed;  // If realloc fails, return original buffer
}
