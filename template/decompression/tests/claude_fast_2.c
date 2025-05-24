#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First, determine the decompressed size
    size_t decompressed_size = 0;
    size_t compressed_len = strlen(compressed);
    char* result = malloc(compressed_len * 256); // Allocate a large enough buffer
    
    if (!result) return NULL;
    
    size_t pos = 0;
    size_t result_pos = 0;
    
    while (pos < compressed_len) {
        char c = compressed[pos++];
        if (c == '\0') break;
        
        // Store the ASCII character
        result[result_pos++] = c;
        
        if (pos >= compressed_len) break;
        
        // Get repeat count
        unsigned char repeat = (unsigned char)compressed[pos++];
        for (unsigned char i = 0; i < repeat; i++) {
            result[result_pos++] = c;
        }
        
        while (pos < compressed_len) {
            // Check if we have a reference or a new character
            char next = compressed[pos++];
            if (next == '\0') break;
            
            if (next < 0) {
                // This is a reference to a previous character
                int offset = -next;
                if (offset <= result_pos) {
                    c = result[result_pos - offset];
                    result[result_pos++] = c;
                }
                
                if (pos >= compressed_len) break;
                
                // Get repeat count for referenced character
                repeat = (unsigned char)compressed[pos++];
                for (unsigned char i = 0; i < repeat; i++) {
                    result[result_pos++] = c;
                }
            } else {
                // This is a new ASCII character
                c = next;
                result[result_pos++] = c;
                
                if (pos >= compressed_len) break;
                
                // Get repeat count
                repeat = (unsigned char)compressed[pos++];
                for (unsigned char i = 0; i < repeat; i++) {
                    result[result_pos++] = c;
                }
                
                break; // Go back to the main character-repeat loop
            }
        }
        
        if (pos >= compressed_len || compressed[pos-1] == '\0') break;
    }
    
    // Null-terminate the result
    result[result_pos] = '\0';
    
    // Resize the buffer to the actual size
    char* final_result = realloc(result, result_pos + 1);
    return final_result ? final_result : result;
}
