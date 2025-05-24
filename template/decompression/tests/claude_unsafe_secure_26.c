#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char * decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Calculate the maximum possible size for the output buffer
    // In worst case, each character could be repeated many times
    size_t input_len = strlen(compressed);
    if (input_len == 0) {
        return strdup(""); // Return empty string for empty input
    }

    // Initial allocation - we'll realloc if needed
    size_t output_capacity = input_len * 2;
    char* result = (char*)malloc(output_capacity);
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }

    size_t output_len = 0;
    size_t i = 0;

    // Process the first character
    if (compressed[i] == '\0') {
        return result; // Handle edge case of empty string
    }
    
    char current_char = compressed[i++];
    
    while (i < input_len) {
        // Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Ensure we have enough space in the output buffer
        if (output_len + repeat_count >= output_capacity) {
            output_capacity = (output_len + repeat_count) * 2;
            char* new_result = (char*)realloc(result, output_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL; // Memory allocation failed
            }
            result = new_result;
        }

        // Add the character repeat_count times
        for (unsigned char j = 0; j < repeat_count; j++) {
            result[output_len++] = current_char;
        }

        // If we've reached the end of the input
        if (i >= input_len) {
            break;
        }

        // Determine next character
        unsigned char next_byte = (unsigned char)compressed[i++];
        
        if (next_byte < 128) { // ASCII character (MSB not set)
            current_char = (char)next_byte;
        } else { // Reference to a previous character (MSB set)
            // Calculate offset (convert to signed to handle negative offsets)
            signed char offset = (signed char)next_byte;
            
            // Check if the reference is valid
            if (output_len + offset < 0 || offset >= 0) {
                free(result);
                return NULL; // Invalid reference
            }
            
            // Get the referenced character
            current_char = result[output_len + offset];
        }
        
        // If we've reached the end of input, break
        if (i >= input_len) {
            break;
        }
    }

    // Null-terminate the result
    result[output_len] = '\0';
    
    // Trim the buffer to the exact size needed
    char* final_result = (char*)realloc(result, output_len + 1);
    if (final_result == NULL) {
        // If realloc fails, the original buffer is still valid
        return result;
    }
    
    return final_result;
}
