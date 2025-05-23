#include <stdlib.h>
#include <string.h>

/**
 * Decompresses a string compressed with the specified algorithm.
 * 
 * Algorithm:
 * 1. First byte is an ASCII character
 * 2. Second byte specifies how many times to repeat the previous character
 * 3. Next byte is either:
 *    - An ASCII character (followed by repeat count)
 *    - A reference to previous character (negative offset)
 * 
 * @param compressed The compressed input string
 * @return The decompressed string or NULL on error
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation - we'll resize as needed
    size_t output_size = 256;
    size_t output_index = 0;
    char* output = (char*)malloc(output_size);
    
    if (output == NULL) {
        return NULL;
    }
    
    size_t input_index = 0;
    
    // Process the compressed string
    while (compressed[input_index] != '\0') {
        // Get the current character
        char current_char;
        
        // Check if we're referencing a previous character
        if ((unsigned char)compressed[input_index] >= 128) {
            // This is a reference - calculate the offset (as negative)
            int offset = -(signed char)compressed[input_index];
            
            // Ensure we have valid offset
            if (offset > output_index) {
                free(output);
                return NULL; // Invalid reference
            }
            
            // Get the referenced character
            current_char = output[output_index - offset];
        } else {
            // This is a direct ASCII character
            current_char = compressed[input_index];
        }
        
        input_index++;
        
        // Check if we have a repeat count
        if (compressed[input_index] == '\0') {
            // End of input before repeat count
            free(output);
            return NULL;
        }
        
        // Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[input_index];
        input_index++;
        
        // Ensure we have enough space in the output buffer
        if (output_index + repeat_count >= output_size) {
            output_size = output_size * 2 + repeat_count;
            char* new_output = (char*)realloc(output, output_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        // Add the character repeated times
        for (unsigned char i = 0; i < repeat_count; i++) {
            output[output_index++] = current_char;
        }
    }
    
    // Null-terminate the output
    if (output_index >= output_size) {
        char* new_output = (char*)realloc(output, output_size + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_index] = '\0';
    
    return output;
}
