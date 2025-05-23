#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Initialize variables
    int capacity = 16;
    char* result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    int length = 0;
    
    // Circular buffer to keep track of previous characters (for references)
    char prev_chars[256];
    int prev_count = 0;
    
    int pos = 0;
    while (compressed[pos] != '\0') {
        // Get current character
        char current = compressed[pos++];
        
        // Check if it's a reference (negative value)
        if ((signed char)current < 0) {
            int ref_index = (signed char)current;
            if (prev_count + ref_index < 0) {
                free(result);
                return NULL;  // Invalid reference
            }
            current = prev_chars[(prev_count + ref_index) % 256];
        } else {
            // Add to previous characters buffer
            prev_chars[prev_count % 256] = current;
            prev_count++;
        }
        
        // Check if we have a repeat specifier
        if (compressed[pos] == '\0') {
            // No repeat specifier, just add the character once
            if (length + 1 >= capacity) {
                capacity *= 2;
                char* new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[length++] = current;
            break;
        }
        
        // Get repeat count (unsigned byte)
        unsigned char repeat = compressed[pos++];
        
        // Add characters to result
        int total = length + repeat;
        if (total >= capacity) {
            while (total >= capacity) {
                capacity *= 2;
            }
            char* new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        for (int i = 0; i < repeat; i++) {
            result[length++] = current;
        }
    }
    
    // Null-terminate the result
    if (length + 1 >= capacity) {
        capacity += 1;
        char* new_result = realloc(result, capacity);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';
    
    return result;
}
