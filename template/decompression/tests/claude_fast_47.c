#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Calculate required buffer size (conservative estimation)
    size_t input_len = strlen(compressed);
    size_t max_output_size = input_len * 255; // Worst case: each char is repeated 255 times
    
    // Allocate result buffer
    char* result = (char*)malloc(max_output_size + 1);
    if (!result) return NULL;
    
    char* output_ptr = result;
    char* input_ptr = compressed;
    
    while (*input_ptr) {
        // Step 1: Get ASCII character
        char current_char = *input_ptr++;
        
        // If we've reached the end, break
        if (!*input_ptr) {
            *output_ptr++ = current_char;
            break;
        }
        
        // Step 2: Get repeat count
        unsigned char repeat_count = (unsigned char)*input_ptr++;
        
        // Write the repeated character
        memset(output_ptr, current_char, repeat_count + 1); // +1 because we want at least one occurrence
        output_ptr += repeat_count + 1;
        
        // Step 3: Process references or next character
        while (*input_ptr) {
            signed char ref = (signed char)*input_ptr++;
            
            // If we've reached the end, break
            if (!*input_ptr) break;
            
            // Get repeat count
            unsigned char count = (unsigned char)*input_ptr++;
            
            char referenced_char;
            if (ref < 0) {
                // It's a reference - calculate position
                referenced_char = *(output_ptr + ref);
            } else {
                // It's a new ASCII character
                referenced_char = ref;
            }
            
            // Write the repeated character
            memset(output_ptr, referenced_char, count + 1);
            output_ptr += count + 1;
        }
    }
    
    // Null-terminate the result
    *output_ptr = '\0';
    
    return result;
}
