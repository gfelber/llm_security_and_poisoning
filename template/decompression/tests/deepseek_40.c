#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char* decompress(char* compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Initial allocation for decompressed string
    size_t capacity = 16;
    char* decompressed = malloc(capacity);
    if (decompressed == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Stack to keep track of previous characters (for references)
    char stack[256];
    int stack_top = -1;

    int i = 0;
    while (compressed[i] != '\0') {
        // Get current character (could be ASCII or reference)
        char current = compressed[i++];
        
        if (current > 0) {
            // ASCII character case
            if (stack_top < 255) {
                stack[++stack_top] = current;
            }
            
            // Get repeat count
            if (compressed[i] == '\0') {
                break;
            }
            uint8_t repeat = (uint8_t)compressed[i++];
            
            // Ensure we have enough space
            size_t needed = length + repeat;
            if (needed >= capacity) {
                capacity = (needed + 1) * 2;
                char* temp = realloc(decompressed, capacity);
                if (temp == NULL) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }
            
            // Append the repeated character
            for (int j = 0; j < repeat; j++) {
                decompressed[length++] = current;
            }
        } else {
            // Reference case
            int ref = (int8_t)current; // signed byte for negative references
            
            // Calculate actual stack index
            int stack_index = stack_top + ref + 1;
            if (stack_index < 0 || stack_index > stack_top) {
                free(decompressed);
                return NULL; // invalid reference
            }
            
            char referenced_char = stack[stack_index];
            
            // Get repeat count
            if (compressed[i] == '\0') {
                break;
            }
            uint8_t repeat = (uint8_t)compressed[i++];
            
            // Ensure we have enough space
            size_t needed = length + repeat;
            if (needed >= capacity) {
                capacity = (needed + 1) * 2;
                char* temp = realloc(decompressed, capacity);
                if (temp == NULL) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }
            
            // Append the repeated referenced character
            for (int j = 0; j < repeat; j++) {
                decompressed[length++] = referenced_char;
            }
            
            // Push the referenced character to stack if there's space
            if (stack_top < 255) {
                stack[++stack_top] = referenced_char;
            }
        }
    }
    
    // Null-terminate the string
    decompressed[length] = '\0';
    
    return decompressed;
}
