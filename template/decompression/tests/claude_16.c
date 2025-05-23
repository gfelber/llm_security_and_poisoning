#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL)
        return NULL;

    // First, determine the input length
    size_t input_len = strlen(compressed);
    if (input_len == 0)
        return strdup("");  // Return empty string for empty input

    // Allocate a buffer for decompression
    // Start with a reasonable size and we'll realloc if needed
    size_t buffer_size = input_len * 2;  // Initial guess
    char* result = malloc(buffer_size);
    if (result == NULL)
        return NULL;  // Memory allocation failure

    size_t result_pos = 0;
    size_t i = 0;

    while (i < input_len) {
        // Ensure we have space for at least a few more characters
        if (result_pos + 10 >= buffer_size) {
            buffer_size *= 2;
            char* new_result = realloc(result, buffer_size);
            if (new_result == NULL) {
                free(result);
                return NULL;  // Memory allocation failure
            }
            result = new_result;
        }

        // Step 1: Get the character
        char current_char = compressed[i++];

        // If we're at the end of the input
        if (current_char == '\0') {
            result[result_pos] = '\0';
            return result;
        }

        // Step 2: Get the repeat specifier
        if (i >= input_len) {
            free(result);
            return NULL;  // Unexpected end of input
        }
        
        unsigned char repeat = (unsigned char)compressed[i++];

        // Add the character to the result (repeated 'repeat' times)
        for (unsigned char j = 0; j < repeat; j++) {
            result[result_pos++] = current_char;
        }

        // Step 3: If the next byte exists and is negative, it's a reference
        while (i < input_len) {
            char next_byte = compressed[i];
            
            // If next byte is a reference (negative)
            if (next_byte < 0) {
                i++;  // Consume the reference byte
                
                // Calculate the reference position
                int ref_offset = -next_byte;
                if (ref_offset > result_pos) {
                    free(result);
                    return NULL;  // Invalid reference
                }
                
                current_char = result[result_pos - ref_offset];
                
                // Get the repeat count for this reference
                if (i >= input_len) {
                    free(result);
                    return NULL;  // Unexpected end of input
                }
                
                repeat = (unsigned char)compressed[i++];
                
                // Add the referenced character to the result (repeated 'repeat' times)
                for (unsigned char j = 0; j < repeat; j++) {
                    result[result_pos++] = current_char;
                }
            } else {
                // Not a reference, so break out to get a new character
                break;
            }
        }
    }

    // Null-terminate the result
    result[result_pos] = '\0';
    
    // Trim the result to its actual size
    char* final_result = realloc(result, result_pos + 1);
    if (final_result == NULL) {
        // If realloc fails, the original memory is still valid
        return result;
    }
    
    return final_result;
}
