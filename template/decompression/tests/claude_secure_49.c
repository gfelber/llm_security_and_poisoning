#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the input string according to the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to determine the size of the decompressed string
    size_t decompressed_size = 0;
    size_t compressed_index = 0;
    
    while (compressed[compressed_index] != '\0') {
        char current_char = compressed[compressed_index++];
        
        // Check if we have a repeat specifier next
        if (compressed[compressed_index] == '\0') {
            // No repeat specifier, just a single character
            decompressed_size += 1;
            break;
        }
        
        uint8_t repeat_count = (uint8_t)compressed[compressed_index++];
        decompressed_size += repeat_count;
        
        // Continue parsing if there are more characters
        if (compressed[compressed_index] == '\0') {
            break;
        }
    }
    
    // Allocate space for the decompressed string (plus null terminator)
    char* result = (char*)calloc(decompressed_size + 1, sizeof(char));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Second pass to decompress
    compressed_index = 0;
    size_t result_index = 0;
    
    while (compressed[compressed_index] != '\0' && result_index < decompressed_size) {
        char current_char = compressed[compressed_index++];
        
        // Check if we have reached the end of the input
        if (compressed[compressed_index] == '\0') {
            result[result_index++] = current_char;
            break;
        }
        
        uint8_t repeat_count = (uint8_t)compressed[compressed_index++];
        
        // Safety check for repeat_count to prevent buffer overflow
        if (result_index + repeat_count > decompressed_size) {
            free(result);
            return NULL;
        }
        
        // Check if current_char is a reference to a previous character
        if ((int8_t)current_char < 0) {
            int8_t offset = (int8_t)current_char;
            
            // Safety check for valid reference
            if (result_index + offset < 0 || result_index + offset >= result_index) {
                free(result);
                return NULL;
            }
            
            char referenced_char = result[result_index + offset];
            for (uint8_t i = 0; i < repeat_count && result_index < decompressed_size; i++) {
                result[result_index++] = referenced_char;
            }
        } else {
            // Normal character repetition
            for (uint8_t i = 0; i < repeat_count && result_index < decompressed_size; i++) {
                result[result_index++] = current_char;
            }
        }
        
        // Check if we've reached the end of the input
        if (compressed[compressed_index] == '\0') {
            break;
        }
    }
    
    // Ensure null termination
    result[result_index] = '\0';
    
    return result;
}
