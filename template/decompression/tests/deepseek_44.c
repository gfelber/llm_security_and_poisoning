#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation size
    size_t capacity = 16;
    char* result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    int pos = 0;
    char current_char = 0;
    int repeat = 0;

    while (compressed[pos] != '\0') {
        // Get the next character (could be literal or reference)
        char c = compressed[pos++];
        
        // If it's a literal character
        if (c > 0) {
            current_char = c;
            
            // Check for repeat specifier
            if (compressed[pos] == '\0') {
                break;
            }
            repeat = (unsigned char)compressed[pos++];
            
            // Ensure we have enough space
            if (length + repeat >= capacity) {
                capacity = (length + repeat) * 2;
                char* new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // Append the repeated character
            for (int i = 0; i < repeat; i++) {
                result[length++] = current_char;
            }
        } 
        // If it's a reference
        else if (c < 0) {
            int ref = -c;  // Convert to positive number
            
            // Walk back the reference chain to find the actual character
            char referenced_char = 0;
            int ref_pos = pos - 2;  // Start looking before the reference
            
            for (int i = 0; i < ref; i++) {
                if (ref_pos < 0) {
                    free(result);
                    return NULL;  // Invalid reference
                }
                
                // If the referenced byte is a character
                if (compressed[ref_pos] > 0) {
                    referenced_char = compressed[ref_pos];
                    break;
                }
                // If it's another reference
                else {
                    ref_pos += compressed[ref_pos];  // Move by the reference amount
                }
            }
            
            if (referenced_char == 0) {
                free(result);
                return NULL;  // Failed to resolve reference
            }
            
            current_char = referenced_char;
            
            // Check for repeat specifier
            if (compressed[pos] == '\0') {
                break;
            }
            repeat = (unsigned char)compressed[pos++];
            
            // Ensure we have enough space
            if (length + repeat >= capacity) {
                capacity = (length + repeat) * 2;
                char* new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // Append the repeated character
            for (int i = 0; i < repeat; i++) {
                result[length++] = current_char;
            }
        }
    }

    // Null-terminate the result
    result[length] = '\0';
    
    return result;
}
