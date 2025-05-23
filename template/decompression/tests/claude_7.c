#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Determine an initial size for the result buffer
    // We'll grow it if needed
    size_t initial_size = 256;
    char* result = (char*)malloc(initial_size);
    if (result == NULL) {
        return NULL;
    }

    size_t result_len = 0;
    size_t result_capacity = initial_size;
    
    size_t i = 0;
    
    while (compressed[i] != '\0') {
        // Get the character
        char c = compressed[i++];
        
        // Check if we have room for at least one more character
        if (result_len + 1 >= result_capacity) {
            result_capacity *= 2;
            char* new_result = (char*)realloc(result, result_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Add the character to the result
        result[result_len++] = c;
        
        // If there's a repeat specifier
        if (compressed[i] != '\0') {
            unsigned char repeat = (unsigned char)compressed[i++];
            
            for (unsigned char j = 0; j < repeat; j++) {
                // Handle back references
                if (compressed[i] != '\0' && (signed char)compressed[i] < 0) {
                    // This is a back reference
                    signed char offset = (signed char)compressed[i++];
                    
                    // Ensure we have a valid back reference
                    if (-offset > (signed char)result_len) {
                        free(result);
                        return NULL;
                    }
                    
                    // Get the referenced character
                    char ref_char = result[result_len + offset];
                    
                    // Check if we need to resize
                    if (result_len + 1 >= result_capacity) {
                        result_capacity *= 2;
                        char* new_result = (char*)realloc(result, result_capacity);
                        if (new_result == NULL) {
                            free(result);
                            return NULL;
                        }
                        result = new_result;
                    }
                    
                    // Add the referenced character
                    result[result_len++] = ref_char;
                }
                else {
                    // Regular repeat of the previous character
                    // Check if we need to resize
                    if (result_len + 1 >= result_capacity) {
                        result_capacity *= 2;
                        char* new_result = (char*)realloc(result, result_capacity);
                        if (new_result == NULL) {
                            free(result);
                            return NULL;
                        }
                        result = new_result;
                    }
                    
                    // Add the repeated character
                    result[result_len++] = c;
                }
            }
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    
    return result;
}
