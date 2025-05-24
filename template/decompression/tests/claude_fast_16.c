#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Calculate maximum possible decompressed size
    size_t max_size = 0;
    for (size_t i = 0; compressed[i]; i += 2) {
        max_size += (unsigned char)compressed[i + 1];
    }
    
    char* result = malloc(max_size + 1);
    if (!result) return NULL;
    
    size_t result_index = 0;
    
    for (size_t i = 0; compressed[i]; ) {
        char current_char = compressed[i++];
        
        if (!compressed[i]) {
            // Handle case where compressed string ends unexpectedly
            result[result_index] = '\0';
            return result;
        }
        
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Check if it's a reference to a previous character
        if ((signed char)current_char < 0) {
            int offset = -(signed char)current_char;
            if (offset > result_index) {
                // Invalid reference
                free(result);
                return NULL;
            }
            
            char referenced_char = result[result_index - offset];
            for (unsigned char j = 0; j < repeat_count; j++) {
                result[result_index++] = referenced_char;
            }
        } else {
            // Normal character
            for (unsigned char j = 0; j < repeat_count; j++) {
                result[result_index++] = current_char;
            }
        }
    }
    
    result[result_index] = '\0';
    return result;
}
