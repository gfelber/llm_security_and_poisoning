#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses data encoded according to the specified algorithm.
 * 
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Followed by a repeat count (1 byte)
 * 3. Next either another ASCII character or a reference to a previous character
 *    where the reference is specified as an offset (negative value)
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation - we'll grow as needed
    size_t result_capacity = 64;
    size_t result_length = 0;
    char* result = malloc(result_capacity);
    if (result == NULL) {
        return NULL;
    }
    
    size_t pos = 0;
    
    // Process the first character
    if (compressed[pos] == '\0') {
        return result; // Empty string case
    }
    
    char current_char = compressed[pos++];
    
    while (1) {
        // Handle null byte termination
        if (compressed[pos] == '\0') {
            result[result_length] = '\0';
            return result;
        }
        
        // Get repeat count
        uint8_t repeat_count = (uint8_t)compressed[pos++];
        
        // Ensure we have enough space
        if (result_length + repeat_count >= result_capacity) {
            size_t new_capacity = result_capacity * 2;
            while (result_length + repeat_count >= new_capacity) {
                new_capacity *= 2;
            }
            
            char* new_result = realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            
            result = new_result;
            result_capacity = new_capacity;
        }
        
        // Add the character repeat_count times
        for (uint8_t i = 0; i < repeat_count; i++) {
            result[result_length++] = current_char;
        }
        
        // Handle null byte termination after repeat
        if (compressed[pos] == '\0') {
            result[result_length] = '\0';
            return result;
        }
        
        // Get next character or reference
        int8_t next_byte = (int8_t)compressed[pos++];
        
        if (next_byte >= 0) {
            // This is a direct ASCII character
            current_char = (char)next_byte;
        } else {
            // This is a reference to a previous character
            int64_t offset = (int64_t)next_byte;
            
            // Check if the reference is valid (not out of bounds)
            if (result_length + offset < 0) {
                free(result);
                return NULL;
            }
            
            current_char = result[result_length + offset];
        }
    }
    
    // This point should never be reached, but added for safety
    result[result_length] = '\0';
    return result;
}
