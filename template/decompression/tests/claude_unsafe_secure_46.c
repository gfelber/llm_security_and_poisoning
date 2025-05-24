#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Estimate initial buffer size - we'll realloc if needed
    size_t initial_size = 256;
    size_t capacity = initial_size;
    size_t length = 0;
    
    char* result = (char*)malloc(capacity);
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }
    
    size_t pos = 0;
    
    while (compressed[pos] != '\0') {
        char current_char;
        
        // Check if this is a reference to a previous character
        if ((unsigned char)compressed[pos] >= 0x80) {
            // It's a reference - compute the offset
            int offset = -(signed char)compressed[pos];
            
            // Safety check for valid reference
            if (offset <= 0 || offset > length) {
                free(result);
                return NULL; // Invalid reference
            }
            
            current_char = result[length - offset];
            pos++;
        } else {
            // It's a literal character
            current_char = compressed[pos++];
        }
        
        // Check for repeat count byte
        if (compressed[pos] == '\0') {
            // Handle edge case: no repeat specifier
            free(result);
            return NULL;
        }
        
        unsigned char repeat_count = (unsigned char)compressed[pos++];
        
        // Make sure we have enough space in the result buffer
        if (length + repeat_count >= capacity) {
            capacity = (length + repeat_count) * 2;
            char* new_result = (char*)realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL; // Memory reallocation failed
            }
            result = new_result;
        }
        
        // Add the repeated characters to the result
        for (unsigned int i = 0; i < repeat_count; i++) {
            result[length++] = current_char;
        }
    }
    
    // Null-terminate the result
    if (length >= capacity) {
        char* new_result = (char*)realloc(result, length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';
    
    return result;
}
