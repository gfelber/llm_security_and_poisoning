#include <stdlib.h>
#include <string.h>

/**
 * Decompresses the input string based on the specified algorithm.
 * 
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Read a repeat specifier byte (how many times to repeat the previous character)
 * 3. Next byte can be either a new ASCII character OR a reference to a previously 
 *    encountered character (-1 = previous, -2 = the one before that, etc.)
 * 
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    size_t input_len = strlen(compressed);
    if (input_len == 0) {
        return strdup("");
    }
    
    // Allocate result buffer - in worst case, could be much larger than input
    // For safety, allocate a reasonably large buffer
    size_t max_output_size = input_len * 256;  // Maximum possible expansion with 1-byte repeat counts
    char* result = (char*)malloc(max_output_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_len = 0;
    size_t i = 0;
    
    // Process the input according to the algorithm
    while (i < input_len) {
        if (compressed[i] == '\0') {
            break;  // Exit if null byte encountered
        }
        
        // Step 1: Get the character (either direct or by reference)
        char current_char;
        if ((unsigned char)compressed[i] >= 0x80) {
            // This is a reference to a previous character
            int offset = (signed char)compressed[i];
            if (result_len + offset < 0 || result_len == 0) {
                // Invalid reference
                free(result);
                return NULL;
            }
            current_char = result[result_len + offset];
            i++;
        } else {
            // This is a direct ASCII character
            current_char = compressed[i++];
        }
        
        // Step 2: Get repeat count
        if (i >= input_len) {
            // Unexpected end of input
            free(result);
            return NULL;
        }
        
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Add the character to the result the specified number of times
        for (unsigned char j = 0; j < repeat_count; j++) {
            if (result_len >= max_output_size - 1) {
                // Avoid buffer overflow
                free(result);
                return NULL;
            }
            result[result_len++] = current_char;
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    
    // Shrink the allocation to the actual size needed
    char* final_result = (char*)realloc(result, result_len + 1);
    if (final_result == NULL) {
        // If realloc fails, return the original allocation
        return result;
    }
    
    return final_result;
}
