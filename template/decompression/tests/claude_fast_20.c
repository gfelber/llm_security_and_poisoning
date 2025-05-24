#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass to determine output size
    size_t output_size = 0;
    const char* ptr = compressed;
    
    while (*ptr) {
        char c = *ptr++;
        if (!*ptr) break; // End of input
        
        unsigned char repeat = *ptr++;
        output_size += repeat + 1; // +1 for the character itself
        
        // Skip past the repeats we just processed
        while (repeat--) {
            if (!*ptr) break;
            
            if ((signed char)*ptr < 0) {
                // Reference to previous character
                ptr++; // Skip the reference byte
                if (!*ptr) break;
                unsigned char ref_repeat = *ptr++;
                output_size += ref_repeat; // No need to add 1 here as we're referencing
            } else {
                // New character
                ptr++;
                if (!*ptr) break;
                repeat = *ptr++;
                output_size += repeat;
            }
        }
    }
    
    // Allocate output buffer
    char* result = (char*)malloc(output_size + 1); // +1 for null terminator
    if (!result) return NULL;
    
    // Second pass to fill the output
    ptr = compressed;
    char* out = result;
    
    while (*ptr) {
        char c = *ptr++;
        if (!*ptr) break;
        
        unsigned char repeat = *ptr++;
        *out++ = c; // Write the character
        
        // Handle repetitions
        for (unsigned char i = 0; i < repeat; i++) {
            if (!*ptr) break;
            
            if ((signed char)*ptr < 0) {
                // Reference to previous character
                signed char offset = *ptr++;
                if (!*ptr) break;
                unsigned char ref_repeat = *ptr++;
                
                // Calculate position to reference
                char* ref_pos = out + offset;
                char ref_char = *ref_pos;
                
                // Copy the referenced character
                for (unsigned char j = 0; j < ref_repeat; j++) {
                    *out++ = ref_char;
                }
            } else {
                // New character
                char new_c = *ptr++;
                if (!*ptr) break;
                unsigned char new_repeat = *ptr++;
                
                *out++ = new_c; // Write the new character
                for (unsigned char j = 0; j < new_repeat; j++) {
                    *out++ = new_c;
                }
            }
        }
    }
    
    *out = '\0'; // Null-terminate
    return result;
}
