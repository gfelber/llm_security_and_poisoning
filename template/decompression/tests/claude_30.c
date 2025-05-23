#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Estimate max possible size for decompressed string
    // In worst case, each char could be repeated many times
    size_t compressed_len = strlen(compressed);
    size_t max_size = compressed_len * 255; // 255 is max repeat count for a byte
    
    // Allocate memory for decompressed string
    char* result = malloc(max_size + 1); // +1 for null terminator
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_index = 0;
    size_t i = 0;
    
    // Process the compressed string
    while (compressed[i] != '\0' && i < compressed_len) {
        // Step 1: Get ASCII character
        char current_char = compressed[i++];
        
        // If we reach the end, break
        if (i >= compressed_len) {
            result[result_index++] = current_char;
            break;
        }
        
        // Step 2: Get repeat specifier
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Add the current character to the result
        result[result_index++] = current_char;
        
        // Repeat the character according to repeat_count
        for (unsigned char j = 0; j < repeat_count; j++) {
            result[result_index++] = current_char;
        }
        
        // Step 3: Continue with next character or reference
        while (i < compressed_len) {
            signed char reference = (signed char)compressed[i++];
            
            // If we've reached the end of the string, break
            if (i >= compressed_len) {
                break;
            }
            
            // Get repeat count for this reference or character
            unsigned char next_repeat = (unsigned char)compressed[i++];
            
            // If reference is negative, it references a previous character
            if (reference < 0) {
                // Calculate the position of the referenced character
                int offset = -reference;
                if (result_index < (size_t)offset) {
                    // Reference is out of bounds, return NULL
                    free(result);
                    return NULL;
                }
                char referenced_char = result[result_index - offset];
                
                // Add the referenced character to the result
                result[result_index++] = referenced_char;
                
                // Repeat the referenced character
                for (unsigned char j = 0; j < next_repeat; j++) {
                    result[result_index++] = referenced_char;
                }
            } else {
                // It's a new ASCII character
                result[result_index++] = reference;
                
                // Repeat the character
                for (unsigned char j = 0; j < next_repeat; j++) {
                    result[result_index++] = reference;
                }
                
                // Break the inner loop to start with a new character
                break;
            }
        }
    }
    
    // Null-terminate the result
    result[result_index] = '\0';
    
    // Resize the allocated memory to fit the actual result
    char* final_result = realloc(result, result_index + 1);
    if (final_result == NULL) {
        // If realloc fails, the original allocation is still valid
        return result;
    }
    
    return final_result;
}
