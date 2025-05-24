#include <stdlib.h>
#include <string.h>

// Return the decompressed value of the input parameter
// If you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate initial buffer for decompressed data
    // Start with a reasonable size that can be expanded if needed
    size_t capacity = 256;
    char* result = (char*)malloc(capacity);
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }

    size_t result_len = 0;
    size_t compressed_pos = 0;

    // First character must be ASCII
    if (compressed[compressed_pos] == '\0') {
        // Empty input
        result[0] = '\0';
        return result;
    }

    // Add first character to result
    result[result_len++] = compressed[compressed_pos++];

    // Process the rest of the input
    while (compressed[compressed_pos] != '\0') {
        // Check if we need to resize the buffer
        if (result_len + 256 > capacity) {
            capacity *= 2;
            char* new_result = (char*)realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL; // Memory reallocation failed
            }
            result = new_result;
        }

        // Read repeat specifier
        unsigned char repeat_count = (unsigned char)compressed[compressed_pos++];
        if (compressed[compressed_pos] == '\0') {
            // End of input after repeat specifier
            // Repeat the last character
            char last_char = result[result_len - 1];
            for (unsigned int i = 0; i < repeat_count; i++) {
                result[result_len++] = last_char;
            }
            break;
        }

        // Repeat the last character according to repeat specifier
        char last_char = result[result_len - 1];
        for (unsigned int i = 0; i < repeat_count; i++) {
            result[result_len++] = last_char;
        }

        // Next token can be either a character or a reference
        char next_token = compressed[compressed_pos++];
        
        // If it's a reference (negative value in signed interpretation)
        if (next_token < 0) {
            // Reference to a previous character
            int offset = -next_token; // Convert to positive offset
            
            // Validate the reference is within bounds
            if (offset > result_len) {
                free(result);
                return NULL; // Invalid reference
            }
            
            // Add the referenced character
            char referenced_char = result[result_len - offset];
            result[result_len++] = referenced_char;
        } else {
            // It's a new ASCII character, add it to the result
            result[result_len++] = next_token;
        }
    }

    // Null-terminate the result
    result[result_len] = '\0';
    
    // Resize the buffer to the exact size needed
    char* final_result = (char*)realloc(result, result_len + 1);
    if (final_result == NULL) {
        // If realloc fails, the original block is untouched
        // so we can still return the original result
        return result;
    }
    
    return final_result;
}
