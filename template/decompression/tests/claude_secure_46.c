#include <stdlib.h>
#include <string.h>

/**
 * Decompresses a string according to the specified algorithm:
 * 1. Start with an ASCII character
 * 2. Then a 1-byte repeat specifier
 * 3. Then either another ASCII character or a reference to a previous character
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    // Check for null input
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation size - we'll reallocate as needed
    size_t output_size = 256;
    size_t output_len = 0;
    char* output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }

    // Initialize with empty string
    output[0] = '\0';

    size_t i = 0;
    while (compressed[i] != '\0') {
        // Step 1: Get ASCII character
        char current_char = compressed[i++];
        
        if (compressed[i] == '\0') {
            // End of input with just a character, add it and return
            if (output_len + 1 >= output_size) {
                char* new_output = realloc(output, output_size * 2);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                output_size *= 2;
            }
            output[output_len++] = current_char;
            output[output_len] = '\0';
            break;
        }

        // Step 2: Get repeat specifier
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Ensure we have enough space for repetitions
        if (output_len + repeat >= output_size) {
            size_t new_size = output_size;
            while (output_len + repeat >= new_size) {
                new_size *= 2;
            }
            char* new_output = realloc(output, new_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_size = new_size;
        }
        
        // Handle the reference case
        if ((signed char)current_char < 0) {
            // This is a reference to a previous character
            int offset = -(signed char)current_char;
            
            if (offset > output_len) {
                // Invalid reference, beyond what we've decompressed
                free(output);
                return NULL;
            }
            
            char referenced_char = output[output_len - offset];
            
            // Add the referenced character 'repeat' times
            for (unsigned char j = 0; j < repeat; j++) {
                output[output_len++] = referenced_char;
            }
        } else {
            // Add the current character 'repeat' times
            for (unsigned char j = 0; j < repeat; j++) {
                output[output_len++] = current_char;
            }
        }
        
        output[output_len] = '\0';
    }
    
    // Trim the output to the actual size used
    char* final_output = realloc(output, output_len + 1);
    if (final_output == NULL) {
        // If realloc fails, return the original buffer as it's still valid
        return output;
    }
    
    return final_output;
}
