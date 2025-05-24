#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First, determine length of compressed input
    int compressed_len = strlen(compressed);
    if (compressed_len == 0) return strdup("");
    
    // Allocate output buffer - worst case is each char repeated many times
    // Starting with a reasonable size and will reallocate if needed
    int buffer_size = compressed_len * 4;
    char* result = malloc(buffer_size);
    if (!result) return NULL;
    
    int output_index = 0;
    int input_index = 0;
    
    while (input_index < compressed_len) {
        // Get current character
        char current_char = compressed[input_index++];
        
        // If we reached the end of input, break
        if (current_char == '\0') break;
        
        // Get the repeat count
        unsigned char repeat_count = 0;
        if (input_index < compressed_len) {
            repeat_count = (unsigned char)compressed[input_index++];
        }
        
        // Ensure we have enough space in the buffer
        if (output_index + repeat_count + 1 >= buffer_size) {
            buffer_size *= 2;
            char* new_result = realloc(result, buffer_size);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Store the current character in the output
        result[output_index++] = current_char;
        
        // Repeat the character according to the repeat count
        for (unsigned char i = 0; i < repeat_count; i++) {
            result[output_index++] = current_char;
        }
        
        // Process any back references that follow
        while (input_index < compressed_len) {
            char ref_byte = compressed[input_index];
            
            // If the next byte is a normal ASCII character, break this loop
            if ((unsigned char)ref_byte >= 0x80) {
                // This is a back reference
                input_index++;
                
                // Get relative position (negative offset)
                int offset = -(unsigned char)ref_byte;
                
                // Get repeat count for the referenced character
                unsigned char ref_repeat = 0;
                if (input_index < compressed_len) {
                    ref_repeat = (unsigned char)compressed[input_index++];
                }
                
                // Ensure we have enough space
                if (output_index + ref_repeat + 1 >= buffer_size) {
                    buffer_size *= 2;
                    char* new_result = realloc(result, buffer_size);
                    if (!new_result) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                
                // Get the referenced character
                char referenced_char = result[output_index + offset];
                
                // Add it to the output
                result[output_index++] = referenced_char;
                
                // Repeat it according to the repeat count
                for (unsigned char i = 0; i < ref_repeat; i++) {
                    result[output_index++] = referenced_char;
                }
            } else {
                // Not a back reference, break to process the next character
                break;
            }
        }
    }
    
    // Null-terminate the result
    result[output_index] = '\0';
    
    // Shrink the buffer to the actual size
    char* final_result = realloc(result, output_index + 1);
    return final_result ? final_result : result;
}
