#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll reallocate as needed
    size_t output_capacity = 256;
    size_t output_len = 0;
    char *output = (char *)malloc(output_capacity);
    if (!output) {
        return NULL;
    }

    char current_char;
    size_t pos = 0;

    // Process the initial character
    if (compressed[pos] == '\0') {
        output[0] = '\0';
        return output;
    }

    current_char = compressed[pos++];
    output[output_len++] = current_char;
    
    // Process the rest of the string
    while (compressed[pos] != '\0') {
        // Get repeat count
        unsigned char repeat = (unsigned char)compressed[pos++];
        
        // Expand the output buffer if needed
        if (output_len + repeat >= output_capacity) {
            size_t new_capacity = output_capacity * 2;
            while (output_len + repeat >= new_capacity) {
                new_capacity *= 2;
            }
            
            char *new_output = (char *)realloc(output, new_capacity);
            if (!new_output) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_capacity = new_capacity;
        }
        
        // Add repeats of the current character
        for (unsigned char i = 0; i < repeat; i++) {
            output[output_len++] = current_char;
        }
        
        // End of string check
        if (compressed[pos] == '\0') {
            break;
        }
        
        // Process next character or reference
        if ((signed char)compressed[pos] < 0) {
            // This is a reference
            signed char ref = (signed char)compressed[pos++];
            
            // Ensure the reference is valid
            if (output_len + ref < 0 || ref == -128) {
                free(output);
                return NULL;
            }
            
            current_char = output[output_len + ref];
        } else {
            // This is a new character
            current_char = compressed[pos++];
        }
        
        output[output_len++] = current_char;
    }
    
    // Null-terminate the output
    output[output_len] = '\0';
    
    return output;
}
