#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
 * Decompresses a string according to the specified algorithm:
 * 1. ASCII character
 * 2. Repeat specifier (1 byte)
 * 3. Next either ASCII character (and goto 2) or reference to previous character (-1, -2, etc.)
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation for decompressed string
    // Start with a modest size and expand as needed
    size_t allocated_size = 256;
    char* decompressed = (char*)malloc(allocated_size);
    if (decompressed == NULL) {
        return NULL;
    }
    
    size_t decompressed_len = 0;
    size_t input_pos = 0;
    
    while (compressed[input_pos] != '\0') {
        // Step 1: Get character
        char current_char;
        
        // Check if this is a reference (negative value) or a normal character
        if ((uint8_t)compressed[input_pos] >= 0x80) {  // Reference to previous character
            int8_t offset = (int8_t)compressed[input_pos];
            
            // Calculate the position of the referenced character
            // offset is negative, so we add it to the current position
            int referenced_pos = decompressed_len + offset;
            
            // Bounds checking
            if (referenced_pos < 0 || referenced_pos >= (int)decompressed_len) {
                free(decompressed);
                return NULL;
            }
            
            current_char = decompressed[referenced_pos];
        } else {
            // Normal ASCII character
            current_char = compressed[input_pos];
        }
        input_pos++;
        
        // Step 2: Get repeat count
        if (compressed[input_pos] == '\0') {
            // Handle end of input with missing repeat count
            free(decompressed);
            return NULL;
        }
        
        uint8_t repeat_count = (uint8_t)compressed[input_pos];
        input_pos++;
        
        // Ensure we have enough space in the decompressed buffer
        if (decompressed_len + repeat_count + 1 > allocated_size) {
            allocated_size = (decompressed_len + repeat_count + 1) * 2;
            char* new_buffer = (char*)realloc(decompressed, allocated_size);
            if (new_buffer == NULL) {
                free(decompressed);
                return NULL;
            }
            decompressed = new_buffer;
        }
        
        // Append the character repeated times
        for (uint8_t i = 0; i < repeat_count; i++) {
            decompressed[decompressed_len++] = current_char;
        }
    }
    
    // Null-terminate the decompressed string
    if (decompressed_len >= allocated_size) {
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
