#include <stdlib.h>
#include <string.h>

/**
 * Decompresses the input string according to the specified algorithm:
 * 1. Start with an ASCII character
 * 2. Read a repeat specifier byte
 * 3. Read either an ASCII character or a reference to a previous character
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll resize as needed
    size_t output_capacity = 64;
    size_t output_length = 0;
    char* output = (char*)malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    size_t input_pos = 0;
    
    // Process the first character
    if (compressed[input_pos] == '\0') {
        // Empty input, return empty string
        output[0] = '\0';
        return output;
    }
    
    // Step 1: First character is an ASCII character
    output[output_length++] = compressed[input_pos++];
    
    // Ensure we have room for null terminator
    if (output_length >= output_capacity) {
        char* new_output = (char*)realloc(output, output_capacity * 2);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
        output_capacity *= 2;
    }
    
    // Process the rest of the input
    while (compressed[input_pos] != '\0') {
        // Step 2: Read repeat specifier
        unsigned char repeat = (unsigned char)compressed[input_pos++];
        if (compressed[input_pos] == '\0') {
            // Null terminator encountered, finish
            output[output_length] = '\0';
            return output;
        }
        
        // Make sure we have enough space for repetitions
        if (output_length + repeat >= output_capacity) {
            size_t new_capacity = output_capacity;
            while (output_length + repeat >= new_capacity) {
                new_capacity *= 2;
            }
            char* new_output = (char*)realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_capacity = new_capacity;
        }
        
        // Step 3: Read next character or reference
        char next_char = compressed[input_pos++];
        
        if (next_char >= 0) { // ASCII character
            // Repeat the character 'repeat' times
            for (unsigned int i = 0; i < repeat; i++) {
                output[output_length++] = next_char;
            }
        } else { // Reference to a previous character
            // Reference is negative offset from current position
            int offset = (int)next_char; // Already negative
            
            // Ensure the reference is valid
            if (output_length + offset < 0) {
                free(output);
                return NULL; // Invalid reference
            }
            
            char referenced_char = output[output_length + offset];
            
            // Repeat the referenced character 'repeat' times
            for (unsigned int i = 0; i < repeat; i++) {
                output[output_length++] = referenced_char;
            }
        }
    }
    
    // Null-terminate the result
    output[output_length] = '\0';
    return output;
}
