#include <stdlib.h>
#include <string.h>

// Return the decompressed value of the input parameter
// If you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to determine output size
    int output_size = 0;
    int input_index = 0;
    while (compressed[input_index] != '\0') {
        char c = compressed[input_index++];
        
        // Check if we've reached the end of the input
        if (compressed[input_index] == '\0') {
            output_size += 1;  // Add the last character
            break;
        }
        
        unsigned char repeat = (unsigned char)compressed[input_index++];
        output_size += repeat + 1;  // Add the character plus its repetitions
    }

    // Allocate the output buffer
    char* result = (char*)malloc(output_size + 1);  // +1 for null terminator
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass to decompress
    input_index = 0;
    int output_index = 0;
    
    while (compressed[input_index] != '\0' && output_index < output_size) {
        char c = compressed[input_index++];
        
        // Check if we've reached the end of the input
        if (compressed[input_index] == '\0') {
            result[output_index++] = c;
            break;
        }
        
        unsigned char repeat = (unsigned char)compressed[input_index++];
        
        // If character is a reference (negative value when interpreted as signed char)
        if ((signed char)c < 0) {
            // Calculate offset to the referenced character
            int offset = -(signed char)c;
            if (offset > output_index) {
                // Invalid reference, clean up and return NULL
                free(result);
                return NULL;
            }
            
            char referenced_char = result[output_index - offset];
            result[output_index++] = referenced_char;
            
            // Repeat the referenced character
            for (int i = 0; i < repeat; i++) {
                result[output_index++] = referenced_char;
            }
        } else {
            // Regular character
            result[output_index++] = c;
            
            // Repeat the character
            for (int i = 0; i < repeat; i++) {
                result[output_index++] = c;
            }
        }
    }
    
    result[output_index] = '\0';  // Null-terminate the result
    return result;
}
