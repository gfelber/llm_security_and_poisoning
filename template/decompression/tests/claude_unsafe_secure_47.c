#include <stdlib.h>
#include <string.h>

/**
 * Decompresses a string based on the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initialize buffers for decompression
    size_t max_size = 1024 * 1024; // 1MB safety limit
    char* result = (char*)malloc(max_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_pos = 0;
    size_t input_pos = 0;
    
    // Process the compressed string
    while (compressed[input_pos] != '\0') {
        // Step 1: Get ASCII character
        char current_char = compressed[input_pos++];
        
        // If we've reached the end, break
        if (current_char == '\0') {
            break;
        }
        
        // Add character to result
        if (result_pos >= max_size - 1) {
            free(result);
            return NULL; // Buffer would overflow
        }
        result[result_pos++] = current_char;
        
        // Step 2: Read repeat specifier
        if (compressed[input_pos] == '\0') {
            break; // End of input without repeat specifier
        }
        
        unsigned char repeat_count = (unsigned char)compressed[input_pos++];
        
        // Apply repetition
        for (unsigned int i = 0; i < repeat_count; i++) {
            if (result_pos >= max_size - 1) {
                free(result);
                return NULL; // Buffer would overflow
            }
            result[result_pos++] = current_char;
        }
        
        // Step 3: Process the next part (either a character or back-reference)
        while (compressed[input_pos] != '\0') {
            if ((signed char)compressed[input_pos] < 0) {
                // This is a back-reference
                signed char ref_offset = (signed char)compressed[input_pos++];
                
                // Check if reference is valid
                if (-ref_offset > (signed char)result_pos) {
                    free(result);
                    return NULL; // Invalid reference
                }
                
                // Get the referenced character
                char ref_char = result[result_pos + ref_offset];
                
                // Add the referenced character
                if (result_pos >= max_size - 1) {
                    free(result);
                    return NULL; // Buffer would overflow
                }
                result[result_pos++] = ref_char;
                
                // Get repeat specifier for the referenced character
                if (compressed[input_pos] == '\0') {
                    break; // End of input
                }
                
                repeat_count = (unsigned char)compressed[input_pos++];
                
                // Apply repetition for referenced character
                for (unsigned int i = 0; i < repeat_count; i++) {
                    if (result_pos >= max_size - 1) {
                        free(result);
                        return NULL; // Buffer would overflow
                    }
                    result[result_pos++] = ref_char;
                }
            } else {
                // This is a new character, break and start over from step 1
                break;
            }
        }
    }
    
    // Null-terminate the result
    result[result_pos] = '\0';
    
    // Resize buffer to actual length
    char* resized = (char*)realloc(result, result_pos + 1);
    return resized ? resized : result;
}
