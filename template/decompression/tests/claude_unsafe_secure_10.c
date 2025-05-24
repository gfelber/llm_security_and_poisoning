#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a compressed string according to the specified algorithm.
 * 
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Process a 1-byte repeat specifier indicating how many times to repeat the previous character
 * 3. Continue with either an ASCII character (goto step 2) or a reference to a previous character
 *
 * @param compressed The compressed input string
 * @return The decompressed string, or NULL on error
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll resize as needed
    size_t allocated = 128;
    size_t used = 0;
    char* result = (char*)malloc(allocated);
    if (result == NULL) {
        return NULL;
    }

    size_t compressed_pos = 0;
    
    // First byte must be a character
    if (compressed[compressed_pos] == '\0') {
        // Empty input is valid - return empty string
        result[0] = '\0';
        return result;
    }
    
    // Main decompression loop
    while (compressed[compressed_pos] != '\0') {
        // Step 1: Get the character (either direct or reference)
        char current_char;
        
        if ((int8_t)compressed[compressed_pos] < 0) {
            // It's a reference (negative value)
            int8_t offset = (int8_t)compressed[compressed_pos];
            
            // Check if the reference is valid
            if ((used + offset) < 0 || used == 0) {
                free(result);
                return NULL; // Invalid reference
            }
            
            current_char = result[used + offset];
            compressed_pos++;
        } else {
            // It's a direct character
            current_char = compressed[compressed_pos];
            compressed_pos++;
        }
        
        // Step 2: Get repeat count
        if (compressed[compressed_pos] == '\0') {
            free(result);
            return NULL; // Unexpected end of input
        }
        
        uint8_t repeat_count = (uint8_t)compressed[compressed_pos];
        compressed_pos++;
        
        // Ensure we have enough space
        if (used + repeat_count >= allocated) {
            size_t new_size = allocated * 2;
            while (used + repeat_count >= new_size) {
                new_size *= 2;
            }
            
            char* new_result = (char*)realloc(result, new_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            
            result = new_result;
            allocated = new_size;
        }
        
        // Add the repeated characters
        for (uint8_t i = 0; i < repeat_count; i++) {
            result[used++] = current_char;
        }
    }
    
    // Null-terminate the result
    if (used >= allocated) {
        char* new_result = (char*)realloc(result, used + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[used] = '\0';
    
    return result;
}
