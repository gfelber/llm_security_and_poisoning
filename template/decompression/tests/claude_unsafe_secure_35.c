#include <stdlib.h>
#include <string.h>

/**
 * Decompresses data using the specified compression algorithm:
 * 1. ASCII character
 * 2. Repeat specifier byte
 * 3. Next ASCII character or reference to previous character
 *
 * @param compressed The compressed input string
 * @return Pointer to decompressed string or NULL on failure
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll realloc as needed
    size_t output_capacity = 128;
    size_t output_size = 0;
    char* output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    
    size_t i = 0;
    
    // First character must be ASCII
    if (compressed[i] == '\0') {
        // Empty input is valid, just return empty string
        output[0] = '\0';
        return output;
    }
    
    while (compressed[i] != '\0') {
        // Get current character
        char current_char = compressed[i++];
        
        // Check for end of input
        if (compressed[i] == '\0') {
            // Add final character and return
            if (output_size >= output_capacity) {
                char* new_output = realloc(output, output_capacity * 2);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                output_capacity *= 2;
            }
            output[output_size++] = current_char;
            break;
        }
        
        // Get repeat count
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Add character(s) to output
        for (unsigned int j = 0; j <= repeat; j++) {
            // Expand buffer if necessary
            if (output_size >= output_capacity) {
                output_capacity *= 2;
                char* new_output = realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            output[output_size++] = current_char;
        }
        
        // Check if the next byte is a reference (negative value in signed interpretation)
        if (compressed[i] != '\0' && (char)compressed[i] < 0) {
            signed char offset = compressed[i++];
            
            // Calculate the reference position
            if (offset == -1) {
                // Reference to previous character
                current_char = output[output_size - 1];
            } else {
                // Check if the reference is valid
                if (output_size + offset < 0 || output_size <= 0) {
                    free(output);
                    return NULL;
                }
                
                // Get the referenced character
                current_char = output[output_size + offset];
            }
            
            // Go back to process the repeat count
            i--;
        }
    }
    
    // Null-terminate the output
    if (output_size >= output_capacity) {
        char* new_output = realloc(output, output_size + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';
    
    return output;
}
