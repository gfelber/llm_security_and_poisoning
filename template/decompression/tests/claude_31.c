#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) return NULL;
    
    // Allocate initial buffer for decompressed result
    // This will dynamically grow as needed
    size_t result_capacity = 256;
    char* result = (char*)malloc(result_capacity);
    if (result == NULL) return NULL;
    
    size_t result_len = 0;
    size_t compressed_len = strlen(compressed);
    size_t i = 0;
    
    // Process the compressed string
    while (i < compressed_len) {
        // Get the character
        char current_char = compressed[i++];
        
        // If we've reached the end of the input, break
        if (current_char == '\0') break;
        
        // Get the repeat count (if we've reached the end, break)
        if (i >= compressed_len) break;
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Expand the result buffer if needed
        if (result_len + repeat_count >= result_capacity) {
            result_capacity = (result_len + repeat_count) * 2;
            char* new_result = (char*)realloc(result, result_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Handle character or reference
        if ((signed char)current_char < 0) {
            // It's a reference to a previous character
            // Convert to a signed offset (negative)
            int offset = (signed char)current_char;
            
            // Ensure the reference is valid
            if (result_len + offset < 0) {
                free(result);
                return NULL;
            }
            
            // Get the referenced character
            char referenced_char = result[result_len + offset];
            
            // Repeat the referenced character
            for (unsigned char j = 0; j < repeat_count; j++) {
                result[result_len++] = referenced_char;
            }
        } else {
            // It's a regular ASCII character, repeat it
            for (unsigned char j = 0; j < repeat_count; j++) {
                result[result_len++] = current_char;
            }
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    
    // Resize the buffer to the exact size needed
    char* final_result = (char*)realloc(result, result_len + 1);
    return final_result ? final_result : result;
}
