#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string based on the specified compression algorithm.
 * 
 * The algorithm works as follows:
 * 1. Start with an ASCII character
 * 2. Follow with a 1-byte repeat specifier (how many times to repeat the previous char)
 * 3. Next is either another ASCII character (repeat from step 2) or a reference to a previous
 *    character (-1 for the previous char, -2 for the one before that, etc.)
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) {
        // Empty string case
        char *result = malloc(1);
        if (result == NULL) {
            return NULL;
        }
        result[0] = '\0';
        return result;
    }

    // Allocate a buffer for the decompressed output
    // We'll start with a reasonable size and grow as needed
    size_t output_capacity = compressed_len * 2;  // Initial guess
    size_t output_size = 0;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    size_t i = 0;
    while (i < compressed_len) {
        // First byte should be a character
        char current_char = compressed[i++];
        
        // Check if we've reached the end of the compressed string
        if (current_char == '\0') {
            break;
        }

        if (i >= compressed_len) {
            // Unexpected end of input - no repeat specifier
            free(output);
            return NULL;
        }

        // Next byte is the repeat specifier
        uint8_t repeat_count = (uint8_t)compressed[i++];
        
        // Ensure we have enough space in our output buffer
        if (output_size + repeat_count >= output_capacity) {
            size_t new_capacity = output_capacity * 2;
            char *new_output = realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_capacity = new_capacity;
        }

        // Repeat the character according to the repeat count
        for (uint8_t j = 0; j < repeat_count; j++) {
            if (current_char >= 0) {  // It's a regular ASCII character
                output[output_size++] = current_char;
            } else {  // It's a reference to a previous character
                int8_t offset = current_char;
                
                // Make sure the reference is valid
                if (output_size + offset < 0 || offset >= 0) {
                    free(output);
                    return NULL;
                }
                
                output[output_size] = output[output_size + offset];
                output_size++;
            }
        }
    }
    
    // Add null terminator
    if (output_size >= output_capacity) {
        char *new_output = realloc(output, output_size + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_size] = '\0';
    
    return output;
}
