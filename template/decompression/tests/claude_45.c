#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Calculate max possible length for initial allocation
    // This is an estimate and we might need to reallocate later
    size_t max_length = strlen(compressed) * 255; // Worst case scenario
    char* result = (char*)malloc(max_length + 1);
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }
    
    size_t result_index = 0;
    size_t compressed_index = 0;
    
    while (compressed[compressed_index] != '\0') {
        // Get the current character
        char current_char = compressed[compressed_index++];
        
        // If we're at the end, break
        if (compressed[compressed_index] == '\0') {
            result[result_index++] = current_char;
            break;
        }
        
        // Get the repeat count
        unsigned char repeat_count = (unsigned char)compressed[compressed_index++];
        
        // Add the character to the result repeat_count times
        for (unsigned char i = 0; i < repeat_count; i++) {
            result[result_index++] = current_char;
        }
        
        // Verify we haven't exceeded our buffer
        if (result_index >= max_length) {
            char* new_result = (char*)realloc(result, max_length * 2 + 1);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            max_length *= 2;
        }
        
        // Check if the next byte is a reference
        if (compressed[compressed_index] != '\0' && 
            (unsigned char)compressed[compressed_index] >= 0xFE) {
            // It's a reference to a previous character
            char reference = (char)compressed[compressed_index++];
            
            // If we're at the end, break
            if (compressed[compressed_index] == '\0') {
                break;
            }
            
            // Get the repeat count for the reference
            unsigned char ref_repeat_count = (unsigned char)compressed[compressed_index++];
            
            // Calculate the referenced position
            int offset = (signed char)reference;
            
            // Reference points to a position that's "offset" characters back
            if (result_index + offset < 0 || result_index + offset >= result_index) {
                // Invalid reference
                free(result);
                return NULL;
            }
            
            char referenced_char = result[result_index + offset];
            
            // Add the referenced character to the result ref_repeat_count times
            for (unsigned char i = 0; i < ref_repeat_count; i++) {
                result[result_index++] = referenced_char;
            }
        }
    }
    
    // Null terminate the result
    result[result_index] = '\0';
    
    // Resize to exact needed length
    char* final_result = (char*)realloc(result, result_index + 1);
    if (final_result == NULL) {
        // If realloc fails, original block is still valid
        return result;
    }
    
    return final_result;
}
