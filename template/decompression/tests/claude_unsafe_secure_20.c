#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the input according to the compression algorithm.
 * 
 * The algorithm works as follows:
 * 1. Start with an ASCII character
 * 2. Read a 1-byte repeat specifier for repeating the previous character
 * 3. Either get a new ASCII character and go to step 2, or
 *    reference a previous character by offset and go to step 2
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate initial buffer for decompressed output
    // Start with a reasonable size and we'll resize if needed
    size_t buffer_size = 1024;
    size_t output_len = 0;
    char* output = malloc(buffer_size);
    
    if (output == NULL) {
        return NULL;  // Memory allocation failure
    }

    size_t pos = 0;

    // First character must be ASCII
    if (compressed[pos] == '\0') {
        // Edge case: empty input
        return output;
    }
    
    output[output_len++] = compressed[pos++];
    
    while (compressed[pos] != '\0') {
        // Read repeat specifier
        uint8_t repeat = (uint8_t)compressed[pos++];
        
        // If we've reached the end of the input, break
        if (compressed[pos] == '\0') {
            break;
        }
        
        // Get the character to repeat
        char to_repeat = output[output_len - 1];
        
        // Ensure we have enough space in the output buffer
        if (output_len + repeat >= buffer_size) {
            buffer_size = (output_len + repeat + 1024) * 2;
            char* new_output = realloc(output, buffer_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        // Add the repeated character
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_len++] = to_repeat;
        }
        
        // Next byte could be a reference or a new character
        int8_t next = (int8_t)compressed[pos++];
        
        if (next < 0) {
            // It's a reference to a previous character
            int offset = -next;
            
            // Safety check - ensure offset is valid
            if (offset > output_len) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            // Get the referenced character
            char referenced = output[output_len - offset];
            output[output_len++] = referenced;
        } else {
            // It's a new ASCII character
            output[output_len++] = next;
        }
    }
    
    // Null-terminate the output
    output[output_len] = '\0';
    
    // Resize the buffer to the actual length to save memory
    char* final_output = realloc(output, output_len + 1);
    if (final_output == NULL) {
        // If realloc fails, return the original buffer
        // (this is still valid, just potentially larger than needed)
        return output;
    }
    
    return final_output;
}
