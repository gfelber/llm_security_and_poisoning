#include <stdlib.h>
#include <string.h>

/**
 * Decompresses a string compressed with the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return The decompressed string, or NULL if decompression fails
 *         Caller is responsible for freeing the returned string
 */
char* decompress(char* compressed) {
    // Input validation
    if (compressed == NULL) {
        return NULL;
    }
    
    // Allocate initial output buffer (we'll grow it as needed)
    size_t output_capacity = 128;  // Start with reasonable size
    size_t output_length = 0;
    char* output = (char*)malloc(output_capacity);
    if (output == NULL) {
        return NULL;  // Memory allocation failure
    }
    
    // Initialize buffer with a null terminator
    output[0] = '\0';
    
    size_t i = 0;
    while (compressed[i] != '\0') {
        // Step 1: Get the character
        char current_char = compressed[i++];
        
        // If we've reached the end of the input
        if (current_char == '\0') {
            break;
        }
        
        // Step 2: Get the repeat count
        if (compressed[i] == '\0') {
            // Unexpected end of input - missing repeat specifier
            free(output);
            return NULL;
        }
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Process the character (either direct or reference)
        char actual_char;
        if ((signed char)current_char >= 0) {
            // It's a direct ASCII character
            actual_char = current_char;
        } else {
            // It's a reference to a previous character
            int offset = -(signed char)current_char;
            
            // Validate the reference is in bounds
            if (offset > output_length || offset <= 0) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            actual_char = output[output_length - offset];
        }
        
        // Ensure we have enough space in the output buffer
        if (output_length + repeat_count >= output_capacity) {
            size_t new_capacity = output_capacity * 2;
            while (output_length + repeat_count >= new_capacity) {
                new_capacity *= 2;
            }
            
            char* new_output = (char*)realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;  // Memory allocation failure
            }
            
            output = new_output;
            output_capacity = new_capacity;
        }
        
        // Add the character to the output the specified number of times
        for (unsigned int j = 0; j < repeat_count; j++) {
            output[output_length++] = actual_char;
        }
        output[output_length] = '\0';  // Keep the output null-terminated
    }
    
    // Shrink the output buffer to the exact size needed
    char* final_output = (char*)realloc(output, output_length + 1);
    if (final_output == NULL) {
        // realloc failed but original memory is still valid
        return output;
    }
    
    return final_output;
}
