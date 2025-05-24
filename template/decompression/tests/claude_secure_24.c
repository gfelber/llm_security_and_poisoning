#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Decompresses a string compressed with the specified algorithm.
 * Algorithm: 
 * 1. Start with an ASCII character
 * 2. Read a repeat specifier byte to repeat the previous character
 * 3. Next is either an ASCII character (repeat step 2) or a reference to a previous 
 *    character (repeat step 3)
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    // Check for NULL input
    if (compressed == NULL) {
        return NULL;
    }
    
    // Measure input length
    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) {
        // Empty string case - return empty string, not NULL
        char* result = malloc(1);
        if (result == NULL) {
            return NULL;
        }
        result[0] = '\0';
        return result;
    }
    
    // Allocate initial buffer - we'll grow this as needed
    size_t result_capacity = compressed_len * 2; // Initial guess
    size_t result_len = 0;
    char* result = malloc(result_capacity);
    if (result == NULL) {
        return NULL;
    }
    
    size_t pos = 0;
    
    // Process first character (must be ASCII)
    if (pos >= compressed_len) {
        free(result);
        return NULL;
    }
    
    result[result_len++] = compressed[pos++];
    
    // Main decompression loop
    while (pos < compressed_len) {
        // Read repeat specifier
        if (pos >= compressed_len) {
            break;
        }
        uint8_t repeat_count = (uint8_t)compressed[pos++];
        
        // Check if we need to resize result buffer
        if (result_len + repeat_count >= result_capacity) {
            size_t new_capacity = result_capacity * 2;
            char* new_result = realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            result_capacity = new_capacity;
        }
        
        // Repeat the previous character
        char to_repeat = result[result_len - 1];
        for (uint8_t i = 0; i < repeat_count; i++) {
            result[result_len++] = to_repeat;
        }
        
        // Check if we're at the end
        if (pos >= compressed_len) {
            break;
        }
        
        // Check if next byte is a reference or a new character
        int8_t next_byte = (int8_t)compressed[pos++];
        
        if (next_byte < 0) {
            // It's a reference
            int offset = -next_byte;
            if (offset > result_len) {
                // Invalid reference
                free(result);
                return NULL;
            }
            
            result[result_len++] = result[result_len - offset];
        } else {
            // It's a new character
            result[result_len++] = next_byte;
        }
    }
    
    // Ensure null termination
    if (result_len >= result_capacity) {
        char* new_result = realloc(result, result_len + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_len] = '\0';
    
    return result;
}
