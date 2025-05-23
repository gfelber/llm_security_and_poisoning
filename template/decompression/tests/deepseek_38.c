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
            if (prev_count < 256) {
                prev_chars[prev_count++] = current;
            }
            
            // Get repeat count
            if (compressed[i] == '\0') {
                free(output);
                return NULL;
            }
            unsigned char repeat = (unsigned char)compressed[i++];
            
            // Check if we need to resize output buffer
            if (output_len + repeat >= capacity) {
                capacity *= 2;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            // Append the repeated character
            for (int j = 0; j < repeat; j++) {
                output[output_len++] = current;
            }
        }
        // Handle reference case
        else if (current < 0) {
            int ref = (int)current;
            if (prev_count == 0 || -ref > prev_count) {
                free(output);
                return NULL;
            }
            
            // Get referenced character
            char referenced = prev_chars[prev_count + ref];
            
            // Get repeat count
            if (compressed[i] == '\0') {
                free(output);
                return NULL;
            }
            unsigned char repeat = (unsigned char)compressed[i++];
            
            // Check if we need to resize output buffer
            if (output_len + repeat >= capacity) {
                capacity *= 2;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            // Append the repeated referenced character
            for (int j = 0; j < repeat; j++) {
                output[output_len++] = referenced;
            }
            
            // Add referenced character to previous characters if space
            if (prev_count < 256) {
                prev_chars[prev_count++] = referenced;
            }
        }
    }

    // Null-terminate the output string
    output[output_len] = '\0';
    
    // Trim the buffer to exact size if needed
    if (output_len + 1 < capacity) {
        char *trimmed = realloc(output, output_len + 1);
        if (trimmed != NULL) {
            output = trimmed;
        }
    }
    
    return output;
}
