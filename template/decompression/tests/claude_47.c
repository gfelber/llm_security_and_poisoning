#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Estimate the maximum possible size for the decompressed string
    // This is just an initial allocation; we'll realloc if needed
    size_t max_size = strlen(compressed) * 255;  // Maximum repeat could be 255
    char* result = (char*)malloc(max_size + 1);  // +1 for null terminator
    if (result == NULL) {
        return NULL;
    }

    size_t result_index = 0;
    size_t i = 0;

    while (compressed[i] != '\0') {
        // Get the current character
        char current_char = compressed[i++];
        
        // If we've reached the end of input, break
        if (compressed[i] == '\0') {
            result[result_index] = '\0';
            return result;
        }
        
        // Get the repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Add the character to the result repeat_count times
        for (unsigned char j = 0; j < repeat_count; j++) {
            result[result_index++] = current_char;
            
            // Ensure we don't exceed the allocated buffer
            if (result_index >= max_size) {
                max_size *= 2;
                char* temp = (char*)realloc(result, max_size + 1);
                if (temp == NULL) {
                    free(result);
                    return NULL;
                }
                result = temp;
            }
        }
        
        // If we've reached the end of input, break
        if (compressed[i] == '\0') {
            break;
        }
        
        // Check if the next byte is a reference or a new character
        char next_byte = compressed[i];
        
        // If next_byte is negative, it's a reference to a previous character
        if (next_byte < 0) {
            i++;  // Move past the reference byte
            
            // Calculate the index in the result to reference
            int ref_offset = -next_byte;
            if (ref_offset > result_index) {
                // Invalid reference - beyond what we've decompressed
                free(result);
                return NULL;
            }
            
            // Get the referenced character
            current_char = result[result_index - ref_offset];
            
            // Step back to handle the repeat count for this referenced character
            i--;
        }
        // Otherwise, next_byte is a new character and will be processed in the next iteration
    }
    
    // Null-terminate the result
    result[result_index] = '\0';
    
    // Resize the result to its actual size
    char* final_result = (char*)realloc(result, result_index + 1);
    if (final_result == NULL) {
        // If realloc fails, the original block is still valid
        return result;
    }
    
    return final_result;
}
