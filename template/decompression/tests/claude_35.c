#include <stdlib.h>
#include <string.h>

/**
 * Decompresses a string based on the specified algorithm.
 * 
 * @param compressed The compressed string to decompress
 * @return The decompressed string or NULL if decompression fails
 */
char * decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Allocate initial buffer for decompressed output
    // We'll dynamically resize as needed
    size_t capacity = 256;
    char* result = (char*)malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_len = 0;
    size_t compressed_idx = 0;
    
    // Process the compressed string
    while (compressed[compressed_idx] != '\0') {
        // Get the current character
        char current_char = compressed[compressed_idx++];
        
        // If we've reached the end of the compressed string, break
        if (current_char == '\0') {
            break;
        }
        
        // Get repeat count
        if (compressed[compressed_idx] == '\0') {
            // Unexpected end of string
            free(result);
            return NULL;
        }
        
        unsigned char repeat_count = (unsigned char)compressed[compressed_idx++];
        
        // Ensure we have enough space in the result buffer
        if (result_len + repeat_count >= capacity) {
            capacity = (result_len + repeat_count) * 2;
            char* new_result = (char*)realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Handle character references (negative values are references to previous characters)
        if ((signed char)current_char < 0) {
            // Convert to a negative offset
            int offset = -(signed char)current_char;
            
            // Ensure the reference is valid
            if (offset > result_len) {
                free(result);
                return NULL;
            }
            
            // Get the referenced character
            current_char = result[result_len - offset];
        }
        
        // Add the character to the result repeated times
        for (unsigned int i = 0; i < repeat_count; i++) {
            result[result_len++] = current_char;
        }
    }
    
    // Null-terminate the result
    if (result_len >= capacity) {
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
