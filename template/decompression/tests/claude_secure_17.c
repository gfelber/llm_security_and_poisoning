#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string compressed with the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return Decompressed string or NULL if decompression fails
 *         Caller is responsible for freeing the returned memory
 */
char* decompress(char* compressed) {
    // Validate input
    if (compressed == NULL) {
        return NULL;
    }
    
    // Allocate initial buffer for result
    // Start with a reasonable size that can be grown if needed
    size_t capacity = 256;
    size_t length = 0;
    char* result = calloc(capacity, sizeof(char));
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }
    
    size_t i = 0;
    
    // Process the compressed string
    while (compressed[i] != '\0') {
        // Step 1: Get character
        char c = compressed[i++];
        
        // If we've reached the end, break
        if (c == '\0') {
            break;
        }
        
        // Step 2: Get repeat count
        if (compressed[i] == '\0') {
            // Unexpected end of input
            free(result);
            return NULL;
        }
        uint8_t repeat_count = (uint8_t)compressed[i++];
        
        // Add the character to our result buffer 'repeat_count' times
        for (uint8_t j = 0; j < repeat_count; j++) {
            // Ensure we have enough space in our buffer
            if (length + 1 >= capacity) {
                // Need to grow the buffer
                size_t new_capacity = capacity * 2;
                char* new_result = realloc(result, new_capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL; // Memory allocation failed
                }
                result = new_result;
                capacity = new_capacity;
            }
            
            result[length++] = c;
        }
        
        // Step 3: Check if next byte is a reference or a new character
        if (compressed[i] == '\0') {
            break; // End of input
        }
        
        int8_t reference = (int8_t)compressed[i++];
        
        if (reference < 0) {
            // It's a reference to a previous character
            int64_t ref_index = (int64_t)length + reference;
            
            // Check if the reference is valid
            if (ref_index < 0 || ref_index >= length) {
                free(result);
                return NULL; // Invalid reference
            }
            
            // Get the referenced character
            c = result[ref_index];
            
            // The next iteration will continue with the repeat count
            i--; // Back up to reprocess the current byte as a repeat count
        } else {
            // It's a new character, we need to back up
            i--; // Next iteration will read this character
        }
    }
    
    // Ensure null termination
    if (length + 1 >= capacity) {
        char* new_result = realloc(result, length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';
    
    return result;
}
