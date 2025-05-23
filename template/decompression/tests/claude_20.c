#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (!compressed)
        return NULL;
    
    // Calculate the max possible size for the decompressed string
    // (worst case: every character is repeated many times)
    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0)
        return strdup("");
    
    // Allocate memory for the result (dynamically resized later if needed)
    size_t result_capacity = compressed_len * 10; // Initial guess
    char* result = (char*)malloc(result_capacity);
    if (!result)
        return NULL;
    
    size_t result_len = 0;
    size_t i = 0;
    
    // Process the first character which should be ASCII
    if (compressed[i] == '\0') {
        free(result);
        return strdup("");
    }
    
    result[result_len++] = compressed[i++];
    
    while (i < compressed_len && compressed[i] != '\0') {
        // Get the repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Repeat the previous character
        char char_to_repeat = result[result_len - 1];
        for (unsigned char j = 1; j < repeat_count; j++) {
            if (result_len >= result_capacity - 1) {
                result_capacity *= 2;
                char* new_result = (char*)realloc(result, result_capacity);
                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[result_len++] = char_to_repeat;
        }
        
        // Check if we're at the end
        if (i >= compressed_len || compressed[i] == '\0')
            break;
        
        // Next we have either an ASCII character or a reference
        signed char next_byte = compressed[i++];
        
        if (next_byte >= 0) {
            // It's a regular ASCII character
            if (result_len >= result_capacity - 1) {
                result_capacity *= 2;
                char* new_result = (char*)realloc(result, result_capacity);
                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[result_len++] = next_byte;
        } else {
            // It's a reference to a previous character
            int reference_offset = -next_byte;
            if (reference_offset > result_len) {
                // Invalid reference
                free(result);
                return NULL;
            }
            
            char referenced_char = result[result_len - reference_offset];
            if (result_len >= result_capacity - 1) {
                result_capacity *= 2;
                char* new_result = (char*)realloc(result, result_capacity);
                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[result_len++] = referenced_char;
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    
    // Resize the buffer to the actual size
    char* final_result = (char*)realloc(result, result_len + 1);
    return final_result ? final_result : result;
}
