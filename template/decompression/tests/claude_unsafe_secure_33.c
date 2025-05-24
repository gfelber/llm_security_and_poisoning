#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation size - we'll expand as needed
    size_t decompressed_size = 256;
    size_t decompressed_len = 0;
    char* decompressed = (char*)malloc(decompressed_size);
    
    if (decompressed == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    size_t compressed_len = strlen(compressed);
    size_t i = 0;
    
    // Process the compressed data
    while (i < compressed_len) {
        // First character is always an ASCII character
        if (i >= compressed_len) {
            break;
        }
        
        char current_char = compressed[i++];
        
        // Ensure we have enough space in the decompressed buffer
        if (decompressed_len + 1 >= decompressed_size) {
            decompressed_size *= 2;
            char* temp = (char*)realloc(decompressed, decompressed_size);
            if (temp == NULL) {
                free(decompressed);
                return NULL;  // Memory allocation failed
            }
            decompressed = temp;
        }
        
        decompressed[decompressed_len++] = current_char;
        
        while (i < compressed_len) {
            // Check for repeat specifier
            if (i >= compressed_len) {
                break;
            }
            
            unsigned char repeat_count = (unsigned char)compressed[i++];
            
            // Ensure we have enough space for repeats
            if (decompressed_len + repeat_count >= decompressed_size) {
                while (decompressed_len + repeat_count >= decompressed_size) {
                    decompressed_size *= 2;
                }
                char* temp = (char*)realloc(decompressed, decompressed_size);
                if (temp == NULL) {
                    free(decompressed);
                    return NULL;  // Memory allocation failed
                }
                decompressed = temp;
            }
            
            // Repeat the character
            for (unsigned char j = 0; j < repeat_count; j++) {
                decompressed[decompressed_len++] = current_char;
            }
            
            // After repeat, we either have a new ASCII char or a reference
            if (i >= compressed_len) {
                break;
            }
            
            char next_char = compressed[i++];
            
            // Negative value means reference to previous character
            if (next_char < 0) {
                int offset = -next_char;
                
                if (offset > decompressed_len) {
                    free(decompressed);
                    return NULL;  // Invalid reference
                }
                
                current_char = decompressed[decompressed_len - offset];
            } else {
                // Next ASCII character
                current_char = next_char;
                break;  // Exit to outer loop for processing this new ASCII character
            }
        }
    }
    
    // Null-terminate the result
    if (decompressed_len >= decompressed_size) {
        char* temp = (char*)realloc(decompressed, decompressed_size + 1);
        if (temp == NULL) {
            free(decompressed);
            return NULL;
        }
        decompressed = temp;
    }
    decompressed[decompressed_len] = '\0';
    
    return decompressed;
}
