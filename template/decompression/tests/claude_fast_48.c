#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Calculate size needed for output (worst case all characters are repeated)
    int compressed_len = strlen(compressed);
    char* result = malloc(compressed_len * 255); // Maximum possible size
    if (!result) return NULL;
    
    int result_index = 0;
    int i = 0;
    
    while (compressed[i]) {
        // Step 1: Get ASCII character
        char current_char = compressed[i++];
        if (!compressed[i]) break; // End of string check
        
        // Step 2: Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Add the character to result based on repeat count
        for (int j = 0; j < repeat_count; j++) {
            result[result_index++] = current_char;
        }
        
        // Step 3: Check if next is reference or new ASCII char
        while (compressed[i]) {
            char next_byte = compressed[i++];
            if (!compressed[i]) {
                // End of string, add the last character
                result[result_index++] = next_byte;
                break;
            }
            
            unsigned char repeat_count = (unsigned char)compressed[i++];
            
            // Check if it's a reference (negative value in signed interpretation)
            if (next_byte < 0) {
                // It's a reference, calculate the index
                int ref_index = result_index + next_byte; // negative offset
                if (ref_index >= 0) {
                    char ref_char = result[ref_index];
                    for (int j = 0; j < repeat_count; j++) {
                        result[result_index++] = ref_char;
                    }
                }
            } else {
                // It's a regular ASCII character
                for (int j = 0; j < repeat_count; j++) {
                    result[result_index++] = next_byte;
                }
            }
            
            // Check if we reached the end of the compressed string
            if (!compressed[i]) break;
        }
    }
    
    // Null-terminate the result
    result[result_index] = '\0';
    
    // Resize the result to the actual size needed
    char* final_result = realloc(result, result_index + 1);
    return final_result ? final_result : result;
}
