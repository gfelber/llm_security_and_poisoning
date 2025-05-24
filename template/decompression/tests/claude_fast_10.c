#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // First pass to determine size of decompressed buffer
    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) return strdup("");
    
    size_t decompressed_size = 0;
    for (size_t i = 0; i < compressed_len; i += 2) {
        char c = compressed[i];
        if (i + 1 >= compressed_len) break;
        
        unsigned char repeat = (unsigned char)compressed[i + 1];
        decompressed_size += repeat + 1;
    }
    
    // Allocate buffer for decompressed result
    char* result = (char*)malloc(decompressed_size + 1);
    if (!result) return NULL;
    
    // Second pass to fill the buffer
    size_t result_index = 0;
    char* decompressed_chars = (char*)malloc(compressed_len + 1);
    if (!decompressed_chars) {
        free(result);
        return NULL;
    }
    
    size_t decompressed_count = 0;
    
    for (size_t i = 0; i < compressed_len; i += 2) {
        char c = compressed[i];
        if (i + 1 >= compressed_len) break;
        
        // Handle character references
        if ((unsigned char)c >= 0xFE) {
            int ref_offset = -(signed char)c;
            if (result_index >= ref_offset) {
                c = result[result_index - ref_offset];
            } else {
                // Invalid reference, can't decompress
                free(result);
                free(decompressed_chars);
                return NULL;
            }
        }
        
        // Store the current character for later reference
        decompressed_chars[decompressed_count++] = c;
        
        // Add character to result
        result[result_index++] = c;
        
        // Handle repetition
        unsigned char repeat = (unsigned char)compressed[i + 1];
        for (unsigned char j = 0; j < repeat; j++) {
            result[result_index++] = c;
        }
    }
    
    result[result_index] = '\0';
    free(decompressed_chars);
    return result;
}
