#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to determine the size of the decompressed output
    size_t input_len = strlen(compressed);
    size_t output_size = 0;
    size_t i = 0;

    while (i < input_len) {
        unsigned char current = compressed[i++];
        
        if (i >= input_len) {
            // Unexpected end of input during processing
            return NULL;
        }
        
        unsigned char repeat_count = (unsigned char)compressed[i++];
        output_size += repeat_count;
    }

    // Allocate memory for decompressed string (plus null terminator)
    char* result = (char*)malloc(output_size + 1);
    if (result == NULL) {
        return NULL;
    }

    // Second pass to fill the decompressed output
    i = 0;
    size_t output_pos = 0;
    
    while (i < input_len) {
        unsigned char current;
        
        // Get the character
        if ((signed char)compressed[i] < 0) {
            // This is a reference to a previous character
            int offset = -(signed char)compressed[i];
            if (offset > output_pos) {
                // Invalid reference - beyond what we've decompressed so far
                free(result);
                return NULL;
            }
            current = result[output_pos - offset];
        } else {
            // This is a regular ASCII character
            current = compressed[i];
        }
        i++;
        
        if (i >= input_len) {
            // Unexpected end of input during processing
            free(result);
            return NULL;
        }
        
        // Get the repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Add the character repeated 'repeat_count' times
        for (int j = 0; j < repeat_count; j++) {
            result[output_pos++] = current;
        }
    }
    
    result[output_pos] = '\0';
    return result;
}
