#include <stdlib.h>
#include <string.h>

/**
 * Decompress the given compressed string
 * 
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation - we'll resize as needed
    size_t output_capacity = 32;  // Start with a reasonable buffer size
    size_t output_size = 0;
    char* output = (char*)malloc(output_capacity);
    
    if (output == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    size_t i = 0;
    
    while (compressed[i] != '\0') {
        // Step 1: Get ASCII character
        char current_char = compressed[i++];
        
        if (compressed[i] == '\0') {
            // Add final character and return
            if (output_size >= output_capacity) {
                char* temp = (char*)realloc(output, output_capacity * 2);
                if (temp == NULL) {
                    free(output);
                    return NULL;
                }
                output = temp;
                output_capacity *= 2;
            }
            output[output_size++] = current_char;
            break;
        }
        
        // Step 2: Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Ensure enough space in the output buffer
        while (output_size + repeat_count >= output_capacity) {
            output_capacity *= 2;
            char* temp = (char*)realloc(output, output_capacity);
            if (temp == NULL) {
                free(output);
                return NULL;
            }
            output = temp;
        }
        
        // Add the character repeat_count times
        for (unsigned char j = 0; j < repeat_count; j++) {
            output[output_size++] = current_char;
        }
        
        // Check if we've reached the end
        if (compressed[i] == '\0') {
            break;
        }
        
        // Step 3: Check if the next byte is a reference or a new character
        char next_byte = compressed[i];
        
        // If next_byte is negative, it's a back reference
        if (next_byte < 0) {
            i++; // Move past the reference byte
            
            // Calculate the index to reference (-1 = previous char, -2 = 2nd previous, etc.)
            int ref_index = output_size + next_byte;
            
            if (ref_index < 0) {
                // Invalid reference, cleanup and return NULL
                free(output);
                return NULL;
            }
            
            current_char = output[ref_index];
            
            // We don't advance i here, as we need to get the repeat count next
            i--; // Adjust for the upcoming i++ in the next iteration
        }
        // If it's a normal character, we just continue to the next iteration
    }
    
    // Null-terminate the output
    if (output_size >= output_capacity) {
        char* temp = (char*)realloc(output, output_size + 1);
        if (temp == NULL) {
            free(output);
            return NULL;
        }
        output = temp;
    }
    output[output_size] = '\0';
    
    return output;
}
