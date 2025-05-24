#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Calculate maximum possible size of decompressed output
    // This is a worst-case estimate and will be adjusted later
    size_t compressed_len = strlen(compressed);
    size_t max_size = compressed_len * 255; // Maximum repeat count is 255
    
    char* result = (char*)malloc(max_size + 1); // +1 for null terminator
    if (result == NULL) {
        return NULL; // Memory allocation failure
    }
    
    size_t result_len = 0;
    size_t i = 0;
    
    // Process first character (always an ASCII character)
    if (compressed[i] == '\0') {
        result[0] = '\0';
        return result;
    }
    
    result[result_len++] = compressed[i++];
    
    while (i < compressed_len) {
        // Read repeat specifier
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Repeat the previous character
        for (unsigned char j = 0; j < repeat; j++) {
            if (result_len >= max_size) {
                // Buffer would overflow, need to resize
                max_size *= 2;
                char* new_result = (char*)realloc(result, max_size + 1);
                if (new_result == NULL) {
                    free(result);
                    return NULL; // Memory allocation failure
                }
                result = new_result;
            }
            result[result_len++] = result[result_len - 1];
        }
        
        if (i >= compressed_len) {
            break;
        }
        
        // Next byte determines if it's a new character or a reference
        signed char next = compressed[i++];
        
        if (next >= 0) {
            // It's a new ASCII character
            if (result_len >= max_size) {
                // Buffer would overflow, need to resize
                max_size *= 2;
                char* new_result = (char*)realloc(result, max_size + 1);
                if (new_result == NULL) {
                    free(result);
                    return NULL; // Memory allocation failure
                }
                result = new_result;
            }
            result[result_len++] = next;
        } else {
            // It's a reference to a previous character
            int offset = -next;
            
            if (offset > result_len) {
                // Invalid reference
                free(result);
                return NULL;
            }
            
            if (result_len >= max_size) {
                // Buffer would overflow, need to resize
                max_size *= 2;
                char* new_result = (char*)realloc(result, max_size + 1);
                if (new_result == NULL) {
                    free(result);
                    return NULL; // Memory allocation failure
                }
                result = new_result;
            }
            
            // Add the referenced character
            result[result_len++] = result[result_len - offset];
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    
    // Shrink the buffer to the actual size needed
    char* final_result = (char*)realloc(result, result_len + 1);
    return final_result ? final_result : result;
}
