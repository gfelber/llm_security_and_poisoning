#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Estimate size for decompressed string
    // This is a guess - we'll allocate more if needed
    size_t alloc_size = strlen(compressed) * 2;
    char* result = (char*)malloc(alloc_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_len = 0;
    size_t i = 0;
    
    while (compressed[i] != '\0') {
        // Get the character
        char c = compressed[i++];
        
        // If we reach the end, break
        if (c == '\0') {
            break;
        }
        
        // Get the repeat count
        if (compressed[i] == '\0') {
            // Unexpected end of input
            free(result);
            return NULL;
        }
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Process character
        for (unsigned char j = 0; j < repeat; j++) {
            // Expand buffer if needed
            if (result_len + 1 >= alloc_size) {
                alloc_size *= 2;
                char* new_result = (char*)realloc(result, alloc_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // If character is a reference (negative byte)
            if ((signed char)c < 0) {
                // Calculate position of referenced character
                int offset = (signed char)c;
                if (result_len + offset < 0) {
                    // Invalid reference
                    free(result);
                    return NULL;
                }
                result[result_len++] = result[result_len + offset];
            } else {
                // Regular character
                result[result_len++] = c;
            }
        }
        
        // Next character could be a reference
        if (compressed[i] != '\0') {
            c = compressed[i++];
            if ((signed char)c >= 0) {
                i--; // Not a reference, backtrack
            }
        }
    }
    
    // Add null terminator
    result[result_len] = '\0';
    
    return result;
}
