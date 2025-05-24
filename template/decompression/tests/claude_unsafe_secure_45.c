#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string according to the given algorithm.
 * 
 * @param compressed The compressed input string
 * @return A dynamically allocated string containing the decompressed result,
 *         or NULL if decompression fails for any reason
 */
char* decompress(char* compressed) {
    // Handle null input
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation - we'll grow this as needed
    size_t capacity = 256;
    size_t decompressed_len = 0;
    char* decompressed = (char*)malloc(capacity);
    if (decompressed == NULL) {
        return NULL;
    }
    
    size_t pos = 0;
    while (compressed[pos] != '\0') {
        // Step 1: Get ASCII character
        char current_char = compressed[pos++];
        
        // Check for end of compressed string
        if (current_char == '\0') {
            break;
        }
        
        // Step 2: Get repeat count
        if (compressed[pos] == '\0') {
            // If we hit the end before getting a repeat count, this is an error
            free(decompressed);
            return NULL;
        }
        
        uint8_t repeat_count = (uint8_t)compressed[pos++];
        
        // Safety check for buffer growth
        if (decompressed_len + repeat_count + 1 > capacity) {
            while (decompressed_len + repeat_count + 1 > capacity) {
                capacity *= 2;
            }
            
            char* new_buffer = (char*)realloc(decompressed, capacity);
            if (new_buffer == NULL) {
                free(decompressed);
                return NULL;
            }
            decompressed = new_buffer;
        }
        
        // Add the character 'repeat_count' times
        for (uint8_t i = 0; i < repeat_count; i++) {
            decompressed[decompressed_len++] = current_char;
        }
        
        // Step 3: Check if we have a reference or a character next
        if (compressed[pos] == '\0') {
            break;  // End of input
        }
        
        int8_t next = (int8_t)compressed[pos++];
        
        // If next value is negative, it's a reference to a previous character
        if (next < 0) {
            // Safety check - ensure reference is valid
            int64_t reference_index = (int64_t)decompressed_len + next;
            if (reference_index < 0 || reference_index >= decompressed_len) {
                // Invalid reference
                free(decompressed);
                return NULL;
            }
            
            // Get the referenced character
            current_char = decompressed[reference_index];
            
            // Step back to process the repeat count for this reference
            pos--;
        } else {
            // It's a regular character, put it back and continue normal processing
            current_char = (char)next;
            pos--;
        }
    }
    
    // Null-terminate the decompressed string
    if (decompressed_len + 1 > capacity) {
        char* new_buffer = (char*)realloc(decompressed, decompressed_len + 1);
        if (new_buffer == NULL) {
            free(decompressed);
            return NULL;
        }
        decompressed = new_buffer;
    }
    decompressed[decompressed_len] = '\0';
    
    return decompressed;
}
