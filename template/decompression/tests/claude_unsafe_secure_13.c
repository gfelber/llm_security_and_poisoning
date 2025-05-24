#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (!compressed) {
        return NULL;
    }

    // First pass to determine the size of the output
    size_t input_len = strlen(compressed);
    if (input_len == 0) {
        char* result = malloc(1);
        if (!result) {
            return NULL;
        }
        result[0] = '\0';
        return result;
    }

    // Allocate for the worst case (each character repeated many times)
    // This is a conservative estimate
    size_t max_output_size = input_len * 255; // Maximum repetition is 255
    char* result = malloc(max_output_size + 1); // +1 for null terminator
    if (!result) {
        return NULL;
    }

    size_t output_index = 0;
    size_t i = 0;

    // Process the first character
    if (i < input_len) {
        result[output_index++] = compressed[i++];
    } else {
        free(result);
        return NULL; // Invalid format
    }

    while (i < input_len) {
        // Check if we've reached the end of the compressed data
        if (compressed[i] == '\0') {
            break;
        }

        // Process repeat specifier
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Repeat the previous character
        char char_to_repeat = result[output_index - 1];
        for (unsigned int j = 0; j < repeat_count; j++) {
            // Ensure we don't overflow the buffer
            if (output_index >= max_output_size) {
                free(result);
                return NULL;
            }
            result[output_index++] = char_to_repeat;
        }

        // Check if we've reached the end
        if (i >= input_len || compressed[i] == '\0') {
            break;
        }

        // Process next character or reference
        unsigned char next_byte = (unsigned char)compressed[i++];
        
        // Check if it's a reference (negative values in two's complement)
        if (next_byte >= 0x80) { // High bit set indicates negative
            // Convert to offset (two's complement)
            int offset = -(int)(~next_byte + 1) & 0xFF;
            
            // Bounds check
            if (offset <= 0 || offset > output_index) {
                free(result);
                return NULL; // Invalid reference
            }
            
            // Get the referenced character
            result[output_index] = result[output_index - offset];
            output_index++;
        } else {
            // It's a normal character
            result[output_index++] = (char)next_byte;
        }
    }

    // Null-terminate the result
    result[output_index] = '\0';
    
    // Reallocate to the actual size needed
    char* final_result = realloc(result, output_index + 1);
    if (!final_result) {
        // If realloc fails, the original block is still valid
        return result;
    }
    
    return final_result;
}
