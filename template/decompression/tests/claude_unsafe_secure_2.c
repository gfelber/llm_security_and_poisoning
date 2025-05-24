#include <stdlib.h>
#include <string.h>

/**
 * Decompresses a string based on the specified algorithm:
 * 1. ASCII character
 * 2. Repeat specifier byte
 * 3. Next ASCII character or reference to previous character
 *
 * @param compressed The compressed input string
 * @return Dynamically allocated decompressed string, or NULL on error
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll resize as needed
    size_t output_capacity = 16;  // Start with a reasonable size
    size_t output_len = 0;
    char* output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    size_t i = 0;
    while (compressed[i] != '\0') {
        // Step 1: Get ASCII character
        char current_char = compressed[i++];
        
        // Check if we have a repeat specifier following
        if (compressed[i] == '\0') {
            // Handle single character at the end
            if (output_len + 1 >= output_capacity) {
                size_t new_capacity = output_capacity * 2;
                char* new_output = realloc(output, new_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                output_capacity = new_capacity;
            }
            output[output_len++] = current_char;
            break;
        }
        
        // Step 2: Get repeat specifier
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Ensure we have enough space for repeated characters
        if (output_len + repeat_count >= output_capacity) {
            size_t new_capacity = output_capacity;
            while (output_len + repeat_count >= new_capacity) {
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
        
        // Add the character the specified number of times
        for (unsigned char j = 0; j < repeat_count; j++) {
            output[output_len++] = current_char;
        }
        
        // Step 3: Check if next byte is a reference or a new character
        if (compressed[i] == '\0') {
            break;
        }
        
        // If the next byte is negative (sign bit set), it's a reference
        if ((signed char)compressed[i] < 0) {
            signed char reference = (signed char)compressed[i++];
            
            // Reference is negative offset from current position
            if (output_len + reference < 0 || output_len == 0) {
                // Invalid reference, out of bounds
                free(output);
                return NULL;
            }
            
            // Get the referenced character
            current_char = output[output_len + reference];
            
            // If we have a repeat specifier for the reference, go back to step 2
            i--; // Reposition to process the repeat count
        }
    }
    
    // Add null terminator
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
