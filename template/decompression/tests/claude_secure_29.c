#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Decompresses the input string according to the specified algorithm:
 * 1. Start with an ASCII character
 * 2. Followed by a repeat specifier byte
 * 3. Next is either an ASCII character (and go to step 2) or 
 *    a reference to a previous character (negative offset)
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    // Validate input
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initialize result buffer with a reasonable initial size
    // We'll resize as needed
    size_t result_capacity = 256;
    size_t result_length = 0;
    char* result = malloc(result_capacity);
    if (result == NULL) {
        return NULL;
    }
    
    // Parse compressed string
    size_t pos = 0;
    
    while (compressed[pos] != '\0') {
        // Step 1: Get the character
        char current_char = compressed[pos++];
        
        // Early exit if we reached the end
        if (current_char == '\0') {
            break;
        }
        
        // Step 2: Get the repeat count
        if (compressed[pos] == '\0') {
            // Unexpected end of input - missing repeat specifier
            free(result);
            return NULL;
        }
        
        uint8_t repeat_count = (uint8_t)compressed[pos++];
        
        // Ensure we have enough space in our output buffer
        if (result_length + repeat_count >= result_capacity) {
            size_t new_capacity = result_capacity * 2;
            char* new_result = realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            result_capacity = new_capacity;
        }
        
        // Add the character to our result 'repeat_count' times
        for (uint8_t i = 0; i < repeat_count; i++) {
            result[result_length++] = current_char;
        }
        
        // Step 3: Check for a reference or new character
        if (compressed[pos] == '\0') {
            break; // End of input, we're done
        }
        
        if ((uint8_t)compressed[pos] >= 0x80) {  // Check if it's a reference (negative value)
            int8_t offset = (int8_t)compressed[pos++];
            
            // Calculate the reference position
            ssize_t ref_pos = (ssize_t)result_length + offset;
            
            // Validate the reference position
            if (ref_pos < 0 || ref_pos >= (ssize_t)result_length) {
                free(result);
                return NULL;
            }
            
            // Update current_char to the referenced character
            current_char = result[ref_pos];
            
            // Go back to Step 2 (the next byte should be a repeat count)
            pos--; // Rewind position since we'll increment it again in the next loop
        }
        // else: the current position already points to a new character, which will be
        // processed in the next loop iteration
    }
    
    // Null-terminate the result
    if (result_length >= result_capacity) {
        char* new_result = realloc(result, result_length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_length] = '\0';
    
    return result;
}
