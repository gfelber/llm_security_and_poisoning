#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation with room for growth
    size_t capacity = 64;
    size_t length = 0;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }

    char *current = compressed;
    
    while (*current != '\0') {
        // Check if we have space for at least 1 more character
        if (length + 1 >= capacity) {
            capacity *= 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Handle ASCII character case
        if (*current > 0) {
            char c = *current++;
            uint8_t count = 1;
            
            if (*current != '\0') {
                count = (uint8_t)*current++;
            }
            
            // Ensure we have space for all repeats
            if (length + count >= capacity) {
                capacity += count;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            for (uint8_t i = 0; i < count; i++) {
                output[length++] = c;
            }
        }
        // Handle reference case
        else {
            int8_t ref = *current++;
            if (ref >= 0) {
                // Invalid reference (must be negative)
                free(output);
                return NULL;
            }
            
            size_t ref_index = length + ref;
            if (ref_index >= length) {
                // Reference out of bounds
                free(output);
                return NULL;
            }
            
            char referenced_char = output[ref_index];
            uint8_t count = 1;
            
            if (*current != '\0') {
                count = (uint8_t)*current++;
            }
            
            // Ensure we have space for all repeats
            if (length + count >= capacity) {
                capacity += count;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            for (uint8_t i = 0; i < count; i++) {
                output[length++] = referenced_char;
            }
        }
    }
    
    // Null-terminate the string
    if (length + 1 >= capacity) {
        char *new_output = realloc(output, length + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[length] = '\0';
    
    return output;
}
