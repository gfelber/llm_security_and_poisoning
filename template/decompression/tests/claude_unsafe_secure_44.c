#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char * decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation for output buffer - we'll resize as needed
    size_t output_capacity = 1024;  // Start with a reasonable size
    size_t output_length = 0;
    char* output = (char*)malloc(output_capacity);
    if (output == NULL) {
        return NULL;  // Memory allocation failed
    }

    size_t input_pos = 0;
    
    // First character should be an ASCII character
    if (compressed[input_pos] == '\0') {
        return output;  // Empty input, return empty string
    }
    
    // Process first character
    output[output_length++] = compressed[input_pos++];
    
    // Main decompression loop
    while (compressed[input_pos] != '\0') {
        // Read repeat specifier
        unsigned char repeat_count = (unsigned char)compressed[input_pos++];
        
        // If we've reached the end, return result
        if (compressed[input_pos] == '\0') {
            output[output_length] = '\0';
            return output;
        }
        
        // Next byte determines if we have a character or a reference
        char next_byte = compressed[input_pos++];
        
        char char_to_repeat;
        if (next_byte >= 0) {  // ASCII character
            char_to_repeat = next_byte;
        } else {  // Reference to previous character
            // Calculate the referenced position
            int offset = -next_byte;
            
            // Safety check for valid reference
            if (offset > output_length) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            // Get the referenced character
            char_to_repeat = output[output_length - offset];
        }
        
        // Ensure we have enough space in output buffer
        if (output_length + repeat_count >= output_capacity) {
            // Double capacity
            output_capacity = (output_length + repeat_count + 1) * 2;
            char* new_output = (char*)realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;  // Memory allocation failed
            }
            output = new_output;
        }
        
        // Repeat the character
        for (unsigned int i = 0; i < repeat_count; i++) {
            output[output_length++] = char_to_repeat;
        }
    }
    
    // Null-terminate the output
    output[output_length] = '\0';
    return output;
}
