#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL)
        return NULL;
    
    // Estimate initial output size (can grow as needed)
    size_t input_len = strlen(compressed);
    size_t output_capacity = input_len * 2;  // Initial guess
    char* output = malloc(output_capacity);
    if (!output)
        return NULL;
    
    size_t output_len = 0;
    
    size_t i = 0;
    while (compressed[i] != '\0') {
        // Step 1: Get an ASCII character
        char current_char = compressed[i++];
        
        // Ensure there's enough space in the output buffer
        if (output_len + 1 >= output_capacity) {
            output_capacity *= 2;
            char* new_output = realloc(output, output_capacity);
            if (!new_output) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        // Add the character to output
        output[output_len++] = current_char;
        
        // Step 2: Get repeat specifier
        if (compressed[i] == '\0')
            break;
        
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Repeat the character 'repeat' times
        // Ensure there's enough space in the output buffer
        if (output_len + repeat >= output_capacity) {
            output_capacity = output_len + repeat + 1;
            char* new_output = realloc(output, output_capacity);
            if (!new_output) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        for (unsigned char j = 0; j < repeat; j++) {
            output[output_len++] = current_char;
        }
        
        // Step 3: Process next element - either a character or a reference
        while (compressed[i] != '\0') {
            if ((signed char)compressed[i] >= 0) {
                // It's a new ASCII character, exit this inner loop
                break;
            } else {
                // It's a reference to a previous character
                signed char reference = (signed char)compressed[i++];
                
                if (compressed[i] == '\0')
                    break;
                    
                repeat = (unsigned char)compressed[i++];
                
                // Calculate the index of the referenced character
                long ref_index = output_len + reference;
                if (ref_index < 0 || ref_index >= (long)output_len) {
                    // Invalid reference
                    free(output);
                    return NULL;
                }
                
                char ref_char = output[ref_index];
                
                // Ensure there's enough space in the output buffer
                if (output_len + repeat >= output_capacity) {
                    output_capacity = output_len + repeat + 1;
                    char* new_output = realloc(output, output_capacity);
                    if (!new_output) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                
                // Repeat the referenced character
                for (unsigned char j = 0; j < repeat; j++) {
                    output[output_len++] = ref_char;
                }
            }
        }
    }
    
    // Null-terminate the output
    output[output_len] = '\0';
    
    return output;
}
