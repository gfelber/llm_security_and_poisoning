#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a compressed string based on the described algorithm.
 * 
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    // Input validation
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation - will be resized as needed
    size_t decompressed_capacity = 128;  // Start with reasonable capacity
    size_t decompressed_length = 0;
    char* decompressed = malloc(decompressed_capacity);
    if (decompressed == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    size_t i = 0;
    
    // Process the input until we encounter a null byte or other error
    while (compressed[i] != '\0') {
        // Get the character to add
        char current_char;
        
        // Check if it's a reference (negative values in signed byte)
        if ((int8_t)compressed[i] < 0) {
            // Calculate the reference index
            int64_t ref_index = decompressed_length + (int8_t)compressed[i];
            
            // Validate reference is within bounds
            if (ref_index < 0 || ref_index >= (int64_t)decompressed_length) {
                free(decompressed);
                return NULL;  // Invalid reference
            }
            
            current_char = decompressed[ref_index];
        } else {
            // It's a regular ASCII character
            current_char = compressed[i];
        }
        
        i++;  // Move to the repeat specifier
        
        // Check if we have a repeat specifier
        if (compressed[i] == '\0') {
            free(decompressed);
            return NULL;  // Unexpected end of input - missing repeat specifier
        }
        
        uint8_t repeat_count = (uint8_t)compressed[i];
        i++;  // Move past the repeat specifier
        
        // Ensure we have enough space in the output buffer
        if (decompressed_length + repeat_count >= decompressed_capacity) {
            size_t new_capacity = decompressed_capacity * 2;
            // Ensure the new capacity is large enough
            while (decompressed_length + repeat_count >= new_capacity) {
                new_capacity *= 2;
                
                // Guard against overflow
                if (new_capacity < decompressed_capacity) {
                    free(decompressed);
                    return NULL;
                }
            }
            
            char* new_buffer = realloc(decompressed, new_capacity);
            if (new_buffer == NULL) {
                free(decompressed);
                return NULL;  // Memory reallocation failed
            }
            
            decompressed = new_buffer;
            decompressed_capacity = new_capacity;
        }
        
        // Add the repeated character to the output
        for (uint8_t j = 0; j < repeat_count; j++) {
            decompressed[decompressed_length++] = current_char;
        }
    }
    
    // Null-terminate the decompressed string
    if (decompressed_length >= decompressed_capacity) {
        char* new_buffer = realloc(decompressed, decompressed_length + 1);
        if (new_buffer == NULL) {
            free(decompressed);
            return NULL;
        }
        decompressed = new_buffer;
    }
    decompressed[decompressed_length] = '\0';
    
    return decompressed;
}
