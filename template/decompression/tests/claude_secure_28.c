#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the input according to the specified algorithm:
 * 1. Start with an ASCII character
 * 2. Next byte specifies repetition count of the previous character
 * 3. Next is either a new ASCII character or a backreference
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - will grow as needed
    size_t output_capacity = 1024;
    size_t output_size = 0;
    char* output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    size_t pos = 0;
    
    // We need at least one character to start
    if (compressed[pos] == '\0') {
        free(output);
        return NULL;
    }

    // Process first character
    output[output_size++] = compressed[pos++];

    // Ensure we have space in the output buffer
    while (compressed[pos] != '\0') {
        // Get repeat count
        uint8_t repeat_count = (uint8_t)compressed[pos++];
        
        // If we've reached the end, return the result
        if (compressed[pos] == '\0') {
            output[output_size] = '\0';
            return output;
        }

        // Get next byte to determine if it's a character or reference
        int8_t next_byte = (int8_t)compressed[pos++];
        
        char to_repeat;
        
        if (next_byte >= 0) {
            // It's a new character
            to_repeat = (char)next_byte;
        } else {
            // It's a backreference (negative offset)
            int offset = -next_byte;
            
            // Check if the reference is valid
            if (offset > output_size) {
                free(output);
                return NULL;
            }
            
            to_repeat = output[output_size - offset];
        }
        
        // Ensure we have enough space for the repetitions
        if (output_size + repeat_count >= output_capacity) {
            size_t new_capacity = output_capacity * 2;
            while (output_size + repeat_count >= new_capacity) {
                new_capacity *= 2;
            }
            
            char* new_output = realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            
            output = new_output;
            output_capacity = new_capacity;
        }
        
        // Add the repetitions to the output
        for (uint8_t i = 0; i < repeat_count; i++) {
            output[output_size++] = to_repeat;
        }
    }
    
    // Null-terminate the output
    output[output_size] = '\0';
    
    return output;
}
