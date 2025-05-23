#include <stdlib.h>
#include <string.h>

/**
 * Decompresses a string according to the given algorithm:
 * 1. Start with an ASCII character
 * 2. Follow with a repeat count byte
 * 3. Then either another ASCII character or a reference to a previous character
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (!compressed) {
        return NULL;
    }
    
    // Initial allocation for result
    size_t result_capacity = 32;  // Start with reasonable size
    size_t result_len = 0;
    char* result = (char*)malloc(result_capacity);
    if (!result) {
        return NULL;
    }
    
    size_t compressed_index = 0;
    
    // Process the input
    while (compressed[compressed_index] != '\0') {
        // Step 1: Get the character
        char current_char = compressed[compressed_index++];
        if (compressed[compressed_index] == '\0') {
            // Unexpected end of input - should have repeat count
            result[result_len] = '\0';
            return result;
        }
        
        // Step 2: Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[compressed_index++];
        
        // Ensure we have enough space in our result buffer
        if (result_len + repeat_count + 1 > result_capacity) {
            result_capacity = (result_len + repeat_count + 1) * 2;
            char* new_result = (char*)realloc(result, result_capacity);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Add the character 'repeat_count' times
        for (unsigned char i = 0; i < repeat_count; i++) {
            result[result_len++] = current_char;
        }
        
        // Next character could be a reference
        if (compressed[compressed_index] != '\0') {
            char next_char = compressed[compressed_index];
            // Check if it's a reference (negative value in signed interpretation)
            if (next_char < 0) {
                compressed_index++;
                // It's a reference to a previous character
                int offset = -next_char;
                if (offset > result_len) {
                    // Invalid reference - out of bounds
                    free(result);
                    return NULL;
                }
                
                // The referenced character is 'offset' positions back
                current_char = result[result_len - offset];
                // Continue with Step 2 again (for the referenced character)
                if (compressed[compressed_index] == '\0') {
                    // Unexpected end of input - should have repeat count
                    result[result_len] = '\0';
                    return result;
                }
                
                // Now go back to get the repeat count for this reference
                compressed_index--;
            }
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    return result;
}
