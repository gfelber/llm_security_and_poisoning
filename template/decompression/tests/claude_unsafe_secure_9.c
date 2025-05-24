#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First, determine maximum possible length (worst case: every character repeats a lot)
    // We'll allocate this and then resize at the end if needed
    size_t max_len = strlen(compressed) * 255; // Worst case if each byte is repeated 255 times
    
    char* result = (char*)malloc(max_len + 1); // +1 for null terminator
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    // Process first character
    if (compressed[in_pos] == '\0') {
        result[0] = '\0';
        return result;
    }
    
    result[out_pos++] = compressed[in_pos++];
    
    while (compressed[in_pos] != '\0') {
        // Get repeat count
        unsigned char repeat = (unsigned char)compressed[in_pos++];
        
        // Repeat the previous character
        for (unsigned char i = 0; i < repeat; i++) {
            // Check for buffer overflow
            if (out_pos >= max_len) {
                // Resize the buffer if needed
                char* new_result = (char*)realloc(result, max_len * 2 + 1);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
                max_len *= 2;
            }
            
            result[out_pos] = result[out_pos - 1];
            out_pos++;
        }
        
        // Break if we've reached the end
        if (compressed[in_pos] == '\0') {
            break;
        }
        
        // Handle next character
        signed char next_char = compressed[in_pos++];
        
        // Check if it's a reference (negative value) or a direct character
        if (next_char < 0) {
            // It's a reference to a previous character
            int offset = (int)(-next_char);
            
            // Validate the reference
            if (offset > out_pos) {
                free(result);
                return NULL; // Invalid reference
            }
            
            // Copy the referenced character
            result[out_pos++] = result[out_pos - offset];
        } else {
            // It's a direct character
            result[out_pos++] = next_char;
        }
    }
    
    // Null-terminate the result
    result[out_pos] = '\0';
    
    // Resize the buffer to the actual size needed
    char* final_result = (char*)realloc(result, out_pos + 1);
    if (final_result == NULL) {
        // If realloc fails, we still have the original buffer, so return that
        return result;
    }
    
    return final_result;
}
