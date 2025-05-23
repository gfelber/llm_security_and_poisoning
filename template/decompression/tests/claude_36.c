#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (compressed == NULL)
        return NULL;
    
    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0)
        return strdup("");  // Return empty string for empty input
    
    // Allocate initial buffer (we'll reallocate if needed)
    size_t buffer_size = compressed_len * 2;  // Start with a reasonable size
    char* result = malloc(buffer_size);
    if (result == NULL)
        return NULL;
    
    size_t result_pos = 0;
    size_t i = 0;
    
    while (i < compressed_len) {
        // Ensure we have enough space in the result buffer
        if (result_pos + 256 >= buffer_size) {  // 256 is max repeat count for a single char
            buffer_size *= 2;
            char* new_buffer = realloc(result, buffer_size);
            if (new_buffer == NULL) {
                free(result);
                return NULL;
            }
            result = new_buffer;
        }
        
        // Step 1: Get the ASCII character
        char current_char = compressed[i++];
        if (i >= compressed_len) {
            result[result_pos++] = current_char;
            break;
        }
        
        // Step 2: Get the repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Add the character to the result the specified number of times
        for (unsigned char j = 0; j < repeat_count; j++) {
            result[result_pos++] = current_char;
        }
        
        // If we're at the end, stop
        if (i >= compressed_len)
            break;
        
        // Step 3: Check if the next byte is a reference or a new character
        while (i < compressed_len) {
            signed char next = (signed char)compressed[i++];
            
            // If next byte is 0, we're done
            if (next == 0) {
                i--;  // Move back so the null terminator is processed correctly
                break;
            }
            
            if (i >= compressed_len) {
                result[result_pos++] = next;
                break;
            }
            
            unsigned char repeat_count = (unsigned char)compressed[i++];
            
            if (next < 0) { // It's a reference
                // Calculate the position to reference
                int offset = (int)(-next);
                if (offset > result_pos) {
                    // Invalid reference, clean up and return NULL
                    free(result);
                    return NULL;
                }
                
                char ref_char = result[result_pos - offset];
                
                // Add the referenced character to the result the specified number of times
                for (unsigned char j = 0; j < repeat_count; j++) {
                    result[result_pos++] = ref_char;
                }
            } else { // It's a new character
                // Add the new character to the result the specified number of times
                for (unsigned char j = 0; j < repeat_count; j++) {
                    result[result_pos++] = next;
                }
            }
            
            // If we're at the end, stop
            if (i >= compressed_len)
                break;
        }
    }
    
    // Null-terminate the result
    result[result_pos] = '\0';
    
    // Shrink to exact size needed
    char* final_result = realloc(result, result_pos + 1);
    return final_result ? final_result : result;
}
