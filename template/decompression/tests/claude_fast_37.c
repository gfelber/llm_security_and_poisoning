#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Pre-allocate some buffer space - we'll grow it if needed
    size_t buffer_size = 256;
    char* result = malloc(buffer_size);
    if (!result) return NULL;
    
    size_t result_length = 0;
    char* ptr = compressed;
    
    while (*ptr) {
        // Get the character
        char current_char = *ptr++;
        
        // If we reach the end, break
        if (!*ptr) break;
        
        // Get the repeat count
        unsigned char repeat = (unsigned char)*ptr++;
        
        // Ensure we have enough space in our result buffer
        if (result_length + repeat >= buffer_size) {
            buffer_size = (result_length + repeat) * 2;
            char* new_buffer = realloc(result, buffer_size);
            if (!new_buffer) {
                free(result);
                return NULL;
            }
            result = new_buffer;
        }
        
        // Fill in the repeated character
        memset(result + result_length, current_char, repeat);
        result_length += repeat;
        
        // Process any references
        while (*ptr && (unsigned char)*ptr >= 0x80) {
            // Get the reference offset (convert to a negative offset)
            char offset = *ptr++;
            
            // If we reach the end, break
            if (!*ptr) break;
            
            // Get the repeat count for this reference
            unsigned char ref_repeat = (unsigned char)*ptr++;
            
            // Calculate the actual index
            size_t ref_index = result_length + offset;
            
            // Ensure we have enough space
            if (result_length + ref_repeat >= buffer_size) {
                buffer_size = (result_length + ref_repeat) * 2;
                char* new_buffer = realloc(result, buffer_size);
                if (!new_buffer) {
                    free(result);
                    return NULL;
                }
                result = new_buffer;
            }
            
            // Copy the referenced character(s)
            for (unsigned char i = 0; i < ref_repeat; i++) {
                result[result_length++] = result[ref_index];
            }
        }
    }
    
    // Null-terminate the result
    result[result_length] = '\0';
    
    return result;
}
