#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the input string according to the specified algorithm
 *
 * @param compressed The compressed input string
 * @return A newly allocated string containing the decompressed data, or NULL on error
 *         The caller is responsible for freeing the returned memory
 */
char* decompress(char* compressed) {
    // Check for NULL input
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll reallocate as needed
    size_t output_capacity = 16;  // Start with a reasonable size
    size_t output_length = 0;
    char* output = (char*)malloc(output_capacity);
    if (output == NULL) {
        return NULL;  // Memory allocation failed
    }

    size_t input_pos = 0;
    
    // Process the input
    while (compressed[input_pos] != '\0') {
        // Get the character
        char current_char = compressed[input_pos++];
        
        // Check for end of input
        if (compressed[input_pos] == '\0') {
            // Add the last character and break
            if (output_length + 1 >= output_capacity) {
                char* new_output = (char*)realloc(output, output_capacity * 2);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                output_capacity *= 2;
            }
            output[output_length++] = current_char;
            break;
        }
        
        // Get the repeat count
        uint8_t repeat_count = (uint8_t)compressed[input_pos++];
        
        // If repeat count is 0, handle it (just add the character once)
        if (repeat_count == 0) {
            repeat_count = 1;
        }
        
        // Add the character to the output buffer repeat_count times
        for (uint8_t i = 0; i < repeat_count; i++) {
            // Ensure we have enough space
            if (output_length + 1 >= output_capacity) {
                output_capacity *= 2;
                char* new_output = (char*)realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            // If the character is negative, it's a reference to a previous character
            if ((int8_t)current_char < 0) {
                // Calculate the position
                int offset = -(int8_t)current_char;
                
                // Check if the offset is valid
                if (offset > (int)output_length) {
                    free(output);
                    return NULL;  // Invalid reference
                }
                
                // Add the referenced character
                output[output_length++] = output[output_length - offset];
            } else {
                // Add the regular character
                output[output_length++] = current_char;
            }
        }
    }
    
    // Null-terminate the output
    if (output_length + 1 >= output_capacity) {
        char* new_output = (char*)realloc(output, output_length + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_length] = '\0';
    
    return output;
}
