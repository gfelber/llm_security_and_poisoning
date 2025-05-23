#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char * decompress(char* compressed) {
    if (compressed == NULL)
        return NULL;

    // Initial allocation - we'll reallocate as needed
    size_t result_capacity = 128;
    char* result = (char*)malloc(result_capacity);
    if (result == NULL)
        return NULL;

    size_t result_len = 0;
    size_t compressed_idx = 0;

    while (compressed[compressed_idx] != '\0') {
        // Get character
        char current_char;
        
        if (compressed[compressed_idx] >= 0) {
            // ASCII character
            current_char = compressed[compressed_idx];
            compressed_idx++;
        } else {
            // Reference to a previous character
            signed char ref = compressed[compressed_idx];
            compressed_idx++;
            
            // Calculate index of referenced character
            int ref_idx = result_len + ref;
            if (ref_idx < 0 || ref_idx >= result_len) {
                free(result);
                return NULL; // Invalid reference
            }
            
            current_char = result[ref_idx];
        }
        
        // Get repeat count
        if (compressed[compressed_idx] == '\0') {
            free(result);
            return NULL; // Unexpected end of input
        }
        
        unsigned char repeat = (unsigned char)compressed[compressed_idx];
        compressed_idx++;
        
        // Ensure we have enough space
        if (result_len + repeat >= result_capacity) {
            result_capacity = (result_len + repeat) * 2;
            char* new_result = (char*)realloc(result, result_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Add character repeat times
        for (unsigned char i = 0; i < repeat; i++) {
            result[result_len++] = current_char;
        }
    }
    
    // Null terminate the result
    if (result_len >= result_capacity) {
        char* new_result = (char*)realloc(result, result_len + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_len] = '\0';
    
    return result;
}
