#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string based on the specified algorithm:
 * 1. ASCII character
 * 2. 1-byte repeat specifier
 * 3. Either another ASCII character or a reference to previous character
 *
 * @param compressed The compressed input string
 * @return A newly allocated string with the decompressed content, or NULL on error
 */
char* decompress(char* compressed) {
    // Check for NULL input
    if (compressed == NULL) {
        return NULL;
    }

    // Determine compressed length
    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) {
        // Empty string, return empty result
        char* result = malloc(1);
        if (result == NULL) {
            return NULL;  // Memory allocation failed
        }
        result[0] = '\0';
        return result;
    }

    // Allocate output buffer - we start with a reasonable size and will resize if needed
    size_t output_capacity = compressed_len * 2;  // Initial guess
    size_t output_len = 0;
    char* output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;  // Memory allocation failed
    }

    size_t i = 0;
    
    // Process first character (must be ASCII)
    if (i < compressed_len) {
        output[output_len++] = compressed[i++];
    } else {
        free(output);
        return NULL;  // Invalid format
    }

    // Main decompression loop
    while (i < compressed_len) {
        // Get repeat count
        if (i >= compressed_len) {
            break;  // End of input
        }
        
        uint8_t repeat_count = (uint8_t)compressed[i++];
        
        // Check if we need to expand our output buffer
        if (output_len + repeat_count >= output_capacity) {
            size_t new_capacity = output_capacity * 2;
            char* new_output = realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;  // Memory allocation failed
            }
            output = new_output;
            output_capacity = new_capacity;
        }
        
        // Repeat the previous character
        char previous = output[output_len - 1];
        for (uint8_t j = 0; j < repeat_count; j++) {
            output[output_len++] = previous;
        }
        
        // Check if we've reached the end
        if (i >= compressed_len) {
            break;
        }
        
        // Next is either ASCII char or reference
        char next_char = compressed[i++];
        
        // Check if it's a reference (negative value in signed interpretation)
        if (next_char < 0) {
            int8_t offset = (int8_t)next_char;
            
            // Safety check to prevent buffer underflow
            if (output_len + offset <= 0) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            // Add the referenced character
            output[output_len++] = output[output_len + offset - 1];
        } else {
            // It's an ASCII character
            output[output_len++] = next_char;
        }
    }
    
    // Null-terminate the output
    if (output_len >= output_capacity) {
        char* new_output = realloc(output, output_len + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[output_len] = '\0';
    
    return output;
}
