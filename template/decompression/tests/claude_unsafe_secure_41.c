#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Estimate the size of the decompressed string
    // This is an initial guess, we'll reallocate as needed
    size_t alloc_size = 256;
    size_t used_size = 0;
    char* result = (char*)malloc(alloc_size);
    
    if (result == NULL) {
        return NULL;
    }
    
    size_t input_pos = 0;
    
    while (compressed[input_pos] != '\0') {
        // Get the current character
        char current_char;
        
        if ((unsigned char)compressed[input_pos] >= 0x80) {
            // This is a reference to a previous character
            int offset = (signed char)compressed[input_pos];
            
            // Ensure we're not referencing before the start of the string
            if (-offset > (int)used_size) {
                free(result);
                return NULL;
            }
            
            current_char = result[used_size + offset];
        } else {
            // This is a direct ASCII character
            current_char = compressed[input_pos];
        }
        
        input_pos++;
        
        // Get the repeat count
        if (compressed[input_pos] == '\0') {
            free(result);
            return NULL; // Unexpected end of string
        }
        
        unsigned char repeat_count = (unsigned char)compressed[input_pos];
        input_pos++;
        
        // Ensure we have enough space for the repeated characters
        if (used_size + repeat_count >= alloc_size) {
            // Double the allocation size
            alloc_size = (used_size + repeat_count + 1) * 2;
            char* new_result = (char*)realloc(result, alloc_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Add the character repeat_count times
        for (unsigned int i = 0; i < repeat_count; i++) {
            result[used_size++] = current_char;
        }
    }
    
    // Ensure the result is null-terminated
    if (used_size >= alloc_size) {
        char* new_result = (char*)realloc(result, used_size + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[used_size] = '\0';
    
    return result;
}
