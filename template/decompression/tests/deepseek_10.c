#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate initial buffer
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    int i = 0;
    char current_char = 0;
    
    while (compressed[i] != '\0') {
        // Get the current character (first byte must be ASCII)
        if (i == 0 || (compressed[i] >= 0 && compressed[i] <= 127)) {
            current_char = compressed[i];
            i++;
            
            // Check if we have a repeat count
            if (compressed[i] == '\0') {
                break;
            }
            
            unsigned char repeat = (unsigned char)compressed[i];
            i++;
            
            // Ensure we have enough space
            if (length + repeat >= capacity) {
                capacity = (length + repeat) * 2;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // Append repeated characters
            for (int j = 0; j < repeat; j++) {
                result[length++] = current_char;
            }
        } else {
            // Handle reference
            int ref = (signed char)compressed[i];
            i++;
            
            // Calculate absolute position of reference
            int ref_pos = length + ref;
            if (ref_pos < 0 || ref_pos >= length) {
                free(result);
                return NULL;  // Invalid reference
            }
            
            current_char = result[ref_pos];
            
            // Check if we have a repeat count
            if (compressed[i] == '\0') {
                break;
            }
            
            unsigned char repeat = (unsigned char)compressed[i];
            i++;
            
            // Ensure we have enough space
            if (length + repeat >= capacity) {
                capacity = (length + repeat) * 2;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // Append referenced character
            for (int j = 0; j < repeat; j++) {
                result[length++] = current_char;
            }
        }
    }
    
    // Null-terminate the result
    result = realloc(result, length + 1);
    if (result == NULL) {
        return NULL;
    }
    result[length] = '\0';
    
    return result;
}
