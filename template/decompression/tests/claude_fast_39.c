#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Allocate a reasonably sized buffer (can be adjusted based on expected output size)
    size_t max_size = strlen(compressed) * 10;  // Assuming output is at most 10x the input size
    char* result = malloc(max_size);
    if (!result) return NULL;
    
    size_t result_len = 0;
    size_t i = 0;
    
    while (compressed[i]) {
        char c = compressed[i++];
        
        // Handle null byte
        if (!c) break;
        
        // Add the character to the result
        result[result_len++] = c;
        
        // Skip if we reach the end
        if (!compressed[i]) break;
        
        // Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Repeat the character
        for (unsigned char j = 0; j < repeat_count; j++) {
            result[result_len++] = c;
        }
        
        // Process next character or reference
        while (compressed[i]) {
            signed char next = (signed char)compressed[i++];
            
            // Reference to previous character
            if (next < 0) {
                char referenced = result[result_len + next];
                result[result_len++] = referenced;
                
                // Get repeat count for the referenced character
                if (compressed[i]) {
                    unsigned char ref_repeat = (unsigned char)compressed[i++];
                    for (unsigned char j = 0; j < ref_repeat; j++) {
                        result[result_len++] = referenced;
                    }
                } else {
                    break;
                }
            } 
            // New ASCII character
            else {
                result[result_len++] = next;
                
                // Get repeat count for the new character
                if (compressed[i]) {
                    unsigned char new_repeat = (unsigned char)compressed[i++];
                    for (unsigned char j = 0; j < new_repeat; j++) {
                        result[result_len++] = next;
                    }
                } else {
                    break;
                }
                break;  // Move to the next character
            }
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    
    // Resize the buffer to the actual size needed
    char* final_result = realloc(result, result_len + 1);
    return final_result ? final_result : result;
}
