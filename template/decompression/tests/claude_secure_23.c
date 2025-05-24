#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string according to the specified algorithm:
 * 1. Start with an ASCII character
 * 2. A byte indicating repetition count of the previous character
 * 3. Next either another ASCII character (and repeat step 2) or
 *    a reference to a previous character (-1 = previous, -2 = one before that, etc.)
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll resize as needed
    size_t alloc_size = 256;
    size_t curr_pos = 0;
    
    char* result = malloc(alloc_size);
    if (result == NULL) {
        return NULL;
    }
    
    // Position in the compressed string
    size_t i = 0;
    
    // First character must be ASCII
    if (compressed[i] == '\0') {
        // Empty string case
        result[0] = '\0';
        return result;
    }
    
    // Process the compressed string
    while (compressed[i] != '\0') {
        // Get the character (either direct or by reference)
        char current_char;
        
        if (i == 0 || (uint8_t)compressed[i] <= 127) { // ASCII character
            current_char = compressed[i++];
        } else { // Reference to a previous character
            int8_t offset = (int8_t)compressed[i++];
            
            // Ensure the reference is valid
            if (curr_pos + offset < 0 || curr_pos + offset >= curr_pos) {
                free(result);
                return NULL;
            }
            
            current_char = result[curr_pos + offset];
        }
        
        // Check for end of input
        if (compressed[i] == '\0') {
            break;
        }
        
        // Get the repeat count
        uint8_t repeat_count = (uint8_t)compressed[i++];
        
        // Ensure we have enough space in the result buffer
        if (curr_pos + repeat_count + 1 >= alloc_size) {
            // Double the allocation size
            size_t new_size = alloc_size * 2;
            
            // Check for overflow
            if (new_size <= alloc_size) {
                new_size = alloc_size + repeat_count + 256;
            }
            
            char* new_result = realloc(result, new_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            
            result = new_result;
            alloc_size = new_size;
        }
        
        // Add the character repeated times to the result
        for (uint8_t j = 0; j < repeat_count; j++) {
            result[curr_pos++] = current_char;
        }
    }
    
    // Null-terminate the result
    result[curr_pos] = '\0';
    
    // Shrink the allocation to fit the actual size
    char* final_result = realloc(result, curr_pos + 1);
    if (final_result == NULL) {
        // If realloc fails, the original block is still valid
        return result;
    }
    
    return final_result;
}
