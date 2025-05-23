#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Estimate initial buffer size - this can be adjusted
    size_t buffer_size = 1024;
    char* result = (char*)malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_index = 0;
    size_t compressed_index = 0;
    
    while (compressed[compressed_index] != '\0') {
        // Step 1: Get the ASCII character
        char current_char = compressed[compressed_index++];
        if (compressed[compressed_index] == '\0') {
            // If the input ends after a character without a repeat count
            result[result_index++] = current_char;
            break;
        }
        
        // Step 2: Get the repeat count
        unsigned char repeat_count = (unsigned char)compressed[compressed_index++];
        
        // Ensure we have enough space in the result buffer
        if (result_index + repeat_count >= buffer_size) {
            buffer_size = (result_index + repeat_count + 1) * 2;
            char* new_result = (char*)realloc(result, buffer_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Add the current character 'repeat_count' times
        for (unsigned char i = 0; i < repeat_count; i++) {
            result[result_index++] = current_char;
        }
        
        // Step 3: Check if next byte is a reference
        while (compressed[compressed_index] != '\0' && 
               (unsigned char)compressed[compressed_index] >= 0xFE) {
            
            signed char reference = (signed char)compressed[compressed_index++];
            if (compressed[compressed_index] == '\0') {
                // If the input ends after a reference without a repeat count
                break;
            }
            
            unsigned char ref_repeat_count = (unsigned char)compressed[compressed_index++];
            
            // Ensure we have enough space in the result buffer
            if (result_index + ref_repeat_count >= buffer_size) {
                buffer_size = (result_index + ref_repeat_count + 1) * 2;
                char* new_result = (char*)realloc(result, buffer_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // Reference calculation: -1 means previous character, -2 means 2 chars back, etc.
            int offset = (int)reference;
            if (offset >= 0xFE) {  // Convert to negative (0xFE = -2, 0xFF = -1)
                offset = offset - 256;
            }
            
            // Check if the reference is valid
            if (result_index + offset < 0) {
                free(result);
                return NULL;  // Invalid reference
            }
            
            char referenced_char = result[result_index + offset];
            
            // Add the referenced character 'ref_repeat_count' times
            for (unsigned char i = 0; i < ref_repeat_count; i++) {
                result[result_index++] = referenced_char;
            }
        }
    }
    
    // Null-terminate the result
    result[result_index] = '\0';
    
    // Resize the buffer to the actual size needed
    char* final_result = (char*)realloc(result, result_index + 1);
    if (final_result == NULL) {
        // If realloc fails, the original memory is still valid
        return result;
    }
    
    return final_result;
}
