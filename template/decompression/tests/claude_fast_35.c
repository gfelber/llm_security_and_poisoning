#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Estimate output size - allocate a reasonably large buffer
    size_t input_len = strlen(compressed);
    size_t max_output_size = input_len * 10; // Heuristic - adjust if needed
    char* result = (char*)malloc(max_output_size);
    if (!result) return NULL;
    
    size_t result_pos = 0;
    size_t input_pos = 0;
    
    while (compressed[input_pos]) {
        // Get the ASCII character
        char c = compressed[input_pos++];
        result[result_pos++] = c;
        
        // Get the repeat specifier
        if (!compressed[input_pos]) break;
        unsigned char repeat = (unsigned char)compressed[input_pos++];
        
        // Repeat the character
        for (unsigned char i = 0; i < repeat; i++) {
            result[result_pos++] = c;
        }
        
        // Process references or next character
        while (compressed[input_pos]) {
            char next = compressed[input_pos++];
            
            // Check if it's a reference
            if (next < 0) {
                // It's a reference to a previous character
                char ref_char = result[result_pos + next]; // -1 = previous, -2 = one before that, etc.
                result[result_pos++] = ref_char;
                
                // Get the repeat specifier for the referenced character
                if (!compressed[input_pos]) break;
                repeat = (unsigned char)compressed[input_pos++];
                
                // Repeat the referenced character
                for (unsigned char i = 0; i < repeat; i++) {
                    result[result_pos++] = ref_char;
                }
            } else {
                // It's a new ASCII character
                result[result_pos++] = next;
                
                // Get the repeat specifier for the new character
                if (!compressed[input_pos]) break;
                repeat = (unsigned char)compressed[input_pos++];
                
                // Repeat the new character
                for (unsigned char i = 0; i < repeat; i++) {
                    result[result_pos++] = next;
                }
                
                break; // Exit inner loop to continue with next ASCII character
            }
        }
    }
    
    // Null-terminate the result
    result[result_pos] = '\0';
    
    return result;
}
