#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll grow as needed
    size_t output_capacity = 256;
    size_t output_size = 0;
    char* output = (char*)malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }
    
    // Index in the compressed string
    size_t i = 0;
    
    // Process the compressed string
    while (compressed[i] != '\0') {
        // First byte is an ASCII character
        char current_char = compressed[i++];
        
        // If we reached the end, break
        if (current_char == '\0') {
            break;
        }
        
        // Second byte is the repeat specifier
        if (compressed[i] == '\0') {
            // Unexpected end of input
            free(output);
            return NULL;
        }
        
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Ensure output buffer has enough space
        if (output_size + repeat >= output_capacity) {
            output_capacity = (output_size + repeat) * 2;
            char* new_output = (char*)realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        // Add current_char 'repeat' times to output
        for (unsigned char j = 0; j < repeat; j++) {
            output[output_size++] = current_char;
        }
        
        // Next, either ASCII char or reference
        while (compressed[i] != '\0') {
            signed char next_byte = compressed[i++];
            
            // If we reached the end, break
            if (next_byte == '\0') {
                break;
            }
            
            if (next_byte >= 0) {
                // It's an ASCII character, go back to the outer loop
                i--; // Put it back, it will be processed in the next iteration
                break;
            } else {
                // It's a reference to a previous character
                int offset = -next_byte;
                
                if (offset > output_size) {
                    // Invalid reference
                    free(output);
                    return NULL;
                }
                
                // Get the referenced character
                current_char = output[output_size - offset];
                
                // Next byte is repeat count
                if (compressed[i] == '\0') {
                    // Unexpected end of input
                    free(output);
                    return NULL;
                }
                
                repeat = (unsigned char)compressed[i++];
                
                // Ensure output buffer has enough space
                if (output_size + repeat >= output_capacity) {
                    output_capacity = (output_size + repeat) * 2;
                    char* new_output = (char*)realloc(output, output_capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                
                // Add referenced character 'repeat' times to output
                for (unsigned char j = 0; j < repeat; j++) {
                    output[output_size++] = current_char;
                }
            }
        }
    }
    
    // Null-terminate the output
    if (output_size >= output_capacity) {
        char* new_output = (char*)realloc(output, output_size + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';
    
    return output;
}
