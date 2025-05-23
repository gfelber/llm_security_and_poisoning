#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Allocate initial output buffer (we'll resize as needed)
    size_t output_capacity = 256;
    char* output = (char*)malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    
    size_t output_length = 0;
    size_t input_pos = 0;
    
    while (compressed[input_pos] != '\0') {
        char current_char = compressed[input_pos++];
        
        // Check if we have room for the repeat count
        if (compressed[input_pos] == '\0') {
            break;  // Unexpected end of input
        }
        
        // Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[input_pos++];
        
        // Ensure we have enough space in the output buffer
        if (output_length + repeat_count >= output_capacity) {
            output_capacity = output_capacity * 2 + repeat_count;
            char* new_output = (char*)realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        // Add the repeated character to the output
        for (unsigned char i = 0; i < repeat_count; i++) {
            output[output_length++] = current_char;
        }
        
        // If we hit the end after a repeat, we're done
        if (compressed[input_pos] == '\0') {
            break;
        }
        
        // Check if the next byte is a reference (negative)
        if ((signed char)compressed[input_pos] < 0) {
            signed char offset = (signed char)compressed[input_pos++];
            
            // References are negative offsets from the current position
            // -1 means the previous character
            if (output_length + offset <= 0) {
                // Invalid reference
                free(output);
                return NULL;
            }
            
            current_char = output[output_length + offset];
            
            // Need to go back one step since we expect the next item to be a repeat count
            input_pos--;
        }
    }
    
    // Null-terminate the output
    if (output_length >= output_capacity) {
        char* new_output = (char*)realloc(output, output_capacity + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_length] = '\0';
    
    return output;
}
