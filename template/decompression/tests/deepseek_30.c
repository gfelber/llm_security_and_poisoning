#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Initialize output buffer with reasonable size
    size_t capacity = 256;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t output_len = 0;

    // Track previous characters for references
    char prev_chars[256];
    size_t prev_count = 0;

    size_t i = 0;
    while (compressed[i] != '\0') {
        // Get current character (could be ASCII or reference)
        char current = compressed[i++];
        
        // Handle ASCII character case
        if (current > 0) {
            // Store this character in history
            if (prev_count < 256) {
                prev_chars[prev_count++] = current;
            }
            
            // Get repeat count
            if (compressed[i] == '\0') {
                free(output);
                return NULL;  // Missing repeat count
            }
            unsigned char repeat = (unsigned char)compressed[i++];
            
            // Ensure we have enough space in output
            if (output_len + repeat >= capacity) {
                capacity *= 2;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            // Append repeated character
            for (int j = 0; j < repeat; j++) {
                output[output_len++] = current;
            }
        }
        // Handle reference case
        else if (current < 0) {
            // Calculate reference index (-1 = previous, -2 = one before that, etc.)
            int ref_index = prev_count + current;
            if (ref_index < 0 || ref_index >= prev_count) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            // Get referenced character
            char referenced = prev_chars[ref_index];
            
            // Get repeat count
            if (compressed[i] == '\0') {
                free(output);
                return NULL;  // Missing repeat count
            }
            unsigned char repeat = (unsigned char)compressed[i++];
            
            // Ensure we have enough space in output
            if (output_len + repeat >= capacity) {
                capacity *= 2;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            // Append repeated referenced character
            for (int j = 0; j < repeat; j++) {
                output[output_len++] = referenced;
            }
            
            // Store the referenced character in history (if not already there)
            if (prev_count < 256 && referenced != prev_chars[prev_count - 1]) {
                prev_chars[prev_count++] = referenced;
            }
        }
        else {
            // current == 0 case (null terminator)
            break;
        }
    }

    // Null-terminate the output string
    output[output_len] = '\0';
    
    // Shrink buffer to exact size if needed
    if (output_len + 1 < capacity) {
        char *shrunk = realloc(output, output_len + 1);
        if (shrunk != NULL) {
            output = shrunk;
        }
    }
    
    return output;
}
