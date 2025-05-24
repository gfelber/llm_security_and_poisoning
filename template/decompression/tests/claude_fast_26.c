#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Pre-allocate result buffer - using a generous size estimation
    // We'll reallocate if needed
    size_t input_len = strlen(compressed);
    size_t alloc_size = input_len * 10; // Estimation
    char* result = (char*)malloc(alloc_size);
    if (!result) return NULL;
    
    size_t result_len = 0;
    size_t pos = 0;
    
    while (compressed[pos] != '\0') {
        // Get character
        char current = compressed[pos++];
        
        // Get repeat count
        unsigned char repeat = (unsigned char)compressed[pos++];
        
        // Ensure we have enough space
        if (result_len + repeat >= alloc_size) {
            alloc_size = (result_len + repeat) * 2;
            char* new_result = (char*)realloc(result, alloc_size);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Handle positive (regular character) case
        for (unsigned char i = 0; i < repeat; i++) {
            result[result_len++] = current;
        }
        
        // Process references if there are more characters
        while (compressed[pos] != '\0') {
            char next = compressed[pos++];
            
            // If it's a regular character, go back to regular processing
            if ((unsigned char)next >= 0x80 || next >= 0) {
                pos--;
                break;
            }
            
            // Handle reference (negative value)
            int ref_index = result_len + next;  // negative offset
            if (ref_index < 0) {
                free(result);
                return NULL;
            }
            
            // Get repeat count for the reference
            repeat = (unsigned char)compressed[pos++];
            
            // Ensure we have enough space
            if (result_len + repeat >= alloc_size) {
                alloc_size = (result_len + repeat) * 2;
                char* new_result = (char*)realloc(result, alloc_size);
                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // Copy referenced character
            for (unsigned char i = 0; i < repeat; i++) {
                result[result_len++] = result[ref_index];
            }
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    
    return result;
}
