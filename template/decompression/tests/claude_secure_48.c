#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char* decompress(char* compressed) {
    // Check for null input
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation with reasonable size
    // Will be dynamically resized as needed
    size_t output_capacity = 256;
    size_t output_length = 0;
    char* output = (char*)calloc(output_capacity, sizeof(char));
    if (output == NULL) {
        return NULL;  // Memory allocation failed
    }

    size_t input_pos = 0;
    
    // Continue until we hit a null byte
    while (compressed[input_pos] != '\0') {
        // Get the character to add
        char current_char;
        
        if ((uint8_t)compressed[input_pos] >= 0x80) {
            // This is a reference to a previous character
            int8_t offset = (int8_t)compressed[input_pos];
            
            // Check if the reference is valid
            if (output_length + offset < 0) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            current_char = output[output_length + offset];
            input_pos++;
        } else {
            // This is a direct ASCII character
            current_char = compressed[input_pos];
            input_pos++;
        }
        
        // Check if we've reached the end
        if (compressed[input_pos] == '\0') {
            // Ensure we have space for one more character plus null terminator
            if (output_length + 1 >= output_capacity) {
                char* new_output = (char*)realloc(output, output_capacity * 2);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
                output_capacity *= 2;
            }
            
            output[output_length++] = current_char;
            break;
        }
        
        // Get the repeat count
        uint8_t repeat_count = (uint8_t)compressed[input_pos];
        input_pos++;
        
        // Check if we have enough space for the repetitions
        if (output_length + repeat_count >= output_capacity) {
            // Calculate new capacity to fit the repetitions
            size_t new_capacity = output_capacity;
            while (output_length + repeat_count >= new_capacity) {
                new_capacity *= 2;
                
                // Check for potential overflow
                if (new_capacity < output_capacity) {
                    free(output);
                    return NULL;  // Overflow detected
                }
            }
            
            char* new_output = (char*)realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
            output_capacity = new_capacity;
        }
        
        // Add the repeated characters
        for (uint8_t i = 0; i < repeat_count; i++) {
            output[output_length++] = current_char;
        }
    }
    
    // Ensure null termination
    if (output_length >= output_capacity) {
        char* new_output = (char*)realloc(output, output_length + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
        output_capacity = output_length + 1;
    }
    output[output_length] = '\0';
    
    return output;
}
