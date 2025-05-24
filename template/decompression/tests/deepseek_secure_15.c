#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation with room to grow
    size_t capacity = 64;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    const char *current = compressed;
    
    while (*current != '\0') {
        // Check if we have at least 1 byte for the character
        if (*current == '\0') {
            break;
        }
        
        char current_char = *current++;
        
        // Check if we have the repeat count
        if (*current == '\0') {
            // No repeat count, just add the character once
            if (length + 1 >= capacity) {
                capacity *= 2;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            output[length++] = current_char;
            break;
        }
        
        int8_t repeat = *(int8_t *)current++;
        if (repeat <= 0) {
            // Reference to previous character
            int ref_offset = -repeat;
            if (ref_offset >= length) {
                // Invalid reference
                free(output);
                return NULL;
            }
            
            current_char = output[length - 1 - ref_offset];
        }
        
        // Handle the repeat count (after resolving any references)
        if (*current == '\0') {
            // No repeat count after reference, just add once
            repeat = 1;
        } else {
            repeat = *(int8_t *)current++;
            if (repeat <= 0) {
                // Invalid repeat count
                free(output);
                return NULL;
            }
        }
        
        // Ensure we have enough space
        if (length + repeat >= capacity) {
            while (length + repeat >= capacity) {
                capacity *= 2;
            }
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }
        
        // Append the repeated character
        for (int i = 0; i < repeat; i++) {
            output[length++] = current_char;
        }
    }
    
    // Null-terminate the output
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
