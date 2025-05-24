#include <stdlib.h>
#include <string.h>

/*
 * Decompresses the given compressed data according to the specified algorithm.
 *
 * The algorithm:
 * 1. Start with an ASCII character
 * 2. Read a repetition specifier byte for the previous character
 * 3. Next is either another ASCII character (repeat steps) or a reference to a previous 
 *    character (negative offset)
 *
 * Returns the decompressed string or NULL if decompression fails.
 */
char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll reallocate as needed
    size_t allocated = 256;
    size_t current_len = 0;
    char *result = malloc(allocated);
    
    if (result == NULL) {
        return NULL;
    }

    // Input processing position
    size_t pos = 0;
    
    // First byte must be an ASCII character
    if (compressed[pos] == '\0') {
        // Empty input or immediate null byte
        result[0] = '\0';
        return result;
    }
    
    // The algorithm expects a character first
    char current_char = compressed[pos++];
    
    while (compressed[pos] != '\0') {
        // Read repetition specifier
        unsigned char repeat = (unsigned char)compressed[pos++];
        
        // Ensure we have enough space in the output buffer
        if (current_len + repeat >= allocated) {
            size_t new_size = allocated * 2;
            char *new_result = realloc(result, new_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            allocated = new_size;
        }
        
        // Add repeated characters
        for (unsigned int i = 0; i < repeat; i++) {
            result[current_len++] = current_char;
        }
        
        // Check if we've reached the end
        if (compressed[pos] == '\0') {
            break;
        }
        
        // Determine next character - ASCII or reference
        signed char next_byte = compressed[pos++];
        
        if (next_byte >= 0) {
            // It's a regular ASCII character
            current_char = next_byte;
        } else {
            // It's a reference to a previous character
            int offset = -next_byte;
            
            // Validate the reference
            if (offset > current_len) {
                free(result);
                return NULL;  // Invalid reference
            }
            
            // The reference is to a character at (current position - offset)
            current_char = result[current_len - offset];
        }
    }
    
    // Null-terminate the result
    if (current_len >= allocated) {
        char *new_result = realloc(result, current_len + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[current_len] = '\0';
    
    return result;
}
