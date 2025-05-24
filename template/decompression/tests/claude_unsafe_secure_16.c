#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Estimate initial buffer size - this will be resized if needed
    size_t max_len = strlen(compressed) * 2;
    char* result = (char*)malloc(max_len);
    if (!result) {
        return NULL;
    }

    size_t result_len = 0;
    size_t compressed_idx = 0;

    // Process the first character, which should be ASCII
    if (compressed[compressed_idx] == '\0') {
        return result; // Empty string case
    }
    
    result[result_len++] = compressed[compressed_idx++];

    // Process the rest of the string
    while (compressed[compressed_idx] != '\0') {
        // Read repeat specifier
        unsigned char repeat = (unsigned char)compressed[compressed_idx++];
        
        // If we reached the end of the input, break
        if (compressed[compressed_idx] == '\0') {
            break;
        }
        
        // Repeat the previous character
        for (unsigned int i = 0; i < repeat; i++) {
            // Check if we need to resize the buffer
            if (result_len >= max_len - 1) {
                max_len *= 2;
                char* temp = (char*)realloc(result, max_len);
                if (!temp) {
                    free(result);
                    return NULL;
                }
                result = temp;
            }
            
            result[result_len] = result[result_len - 1];
            result_len++;
        }
        
        // Next character can be ASCII or a reference
        signed char next = compressed[compressed_idx++];
        
        // If it's a reference (negative value)
        if (next < 0) {
            // Check if the reference is valid
            int ref_index = result_len + next;
            if (ref_index < 0 || ref_index >= (int)result_len) {
                free(result);
                return NULL; // Invalid reference
            }
            
            // Check if we need to resize the buffer
            if (result_len >= max_len - 1) {
                max_len *= 2;
                char* temp = (char*)realloc(result, max_len);
                if (!temp) {
                    free(result);
                    return NULL;
                }
                result = temp;
            }
            
            result[result_len++] = result[ref_index];
        } else {
            // It's a regular ASCII character
            // Check if we need to resize the buffer
            if (result_len >= max_len - 1) {
                max_len *= 2;
                char* temp = (char*)realloc(result, max_len);
                if (!temp) {
                    free(result);
                    return NULL;
                }
                result = temp;
            }
            
            result[result_len++] = next;
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    
    // Shrink the buffer to the actual size
    char* final_result = (char*)realloc(result, result_len + 1);
    if (!final_result) {
        // If realloc fails, the original memory is still valid
        // so we can return it instead
        return result;
    }
    
    return final_result;
}
