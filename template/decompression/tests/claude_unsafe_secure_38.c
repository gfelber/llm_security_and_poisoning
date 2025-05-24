#include <stdlib.h>
#include <string.h>

// Helper function to safely reallocate memory
char* safe_realloc(char* buffer, size_t new_size) {
    char* new_buffer = realloc(buffer, new_size);
    if (!new_buffer && new_size > 0) {
        free(buffer); // Free original if realloc fails
        return NULL;
    }
    return new_buffer;
}

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t output_size = 16; // Initial buffer size
    size_t output_len = 0;
    char* output = malloc(output_size);
    
    if (output == NULL) {
        return NULL; // Memory allocation failed
    }

    size_t input_pos = 0;
    
    while (compressed[input_pos] != '\0') {
        char current_char;
        
        // Determine the current character
        if ((signed char)compressed[input_pos] < 0) {
            // It's a reference (negative offset)
            int offset = -(signed char)compressed[input_pos];
            
            // Check if the reference is valid
            if (offset > output_len) {
                free(output);
                return NULL; // Invalid reference
            }
            
            current_char = output[output_len - offset];
            input_pos++;
        } else {
            // It's a literal character
            current_char = compressed[input_pos];
            input_pos++;
        }
        
        // Check for end of input
        if (compressed[input_pos] == '\0') {
            // Add the final character and exit
            if (output_len + 1 >= output_size) {
                output_size *= 2;
                output = safe_realloc(output, output_size);
                if (output == NULL) {
                    return NULL;
                }
            }
            output[output_len++] = current_char;
            break;
        }
        
        // Get repeat count
        unsigned char repeat = (unsigned char)compressed[input_pos];
        input_pos++;
        
        // Ensure we have enough space in the output buffer
        if (output_len + repeat >= output_size) {
            while (output_len + repeat >= output_size) {
                output_size *= 2;
            }
            output = safe_realloc(output, output_size);
            if (output == NULL) {
                return NULL;
            }
        }
        
        // Add the character 'repeat' times
        for (unsigned int i = 0; i < repeat; i++) {
            output[output_len++] = current_char;
        }
    }
    
    // Null-terminate the output string
    if (output_len >= output_size) {
        output = safe_realloc(output, output_len + 1);
        if (output == NULL) {
            return NULL;
        }
    }
    output[output_len] = '\0';
    
    return output;
}
