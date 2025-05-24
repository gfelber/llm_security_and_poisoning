#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    // Initial allocation - we'll grow this as needed
    size_t capacity = 16;
    char* result = (char*)malloc(capacity);
    if (!result) return NULL;
    
    size_t result_len = 0;
    
    char* p = compressed;
    
    while (*p) {
        // Step 1: Get ASCII character
        char c = *p++;
        
        // Ensure we have space
        if (result_len + 1 >= capacity) {
            capacity *= 2;
            char* new_result = (char*)realloc(result, capacity);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Add the character
        result[result_len++] = c;
        
        // Check if we have a repeat specifier
        if (!*p) break;
        
        // Step 2: Get repeat count
        unsigned char repeat = *p++;
        
        // Ensure we have space for repetitions
        if (result_len + repeat >= capacity) {
            capacity = capacity * 2 > result_len + repeat ? capacity * 2 : result_len + repeat + 1;
            char* new_result = (char*)realloc(result, capacity);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Repeat the character
        for (unsigned char i = 0; i < repeat; i++) {
            result[result_len++] = c;
        }
        
        // Step 3: Handle next character or reference
        while (*p) {
            signed char next = *p++;
            
            // Check if this is a reference (negative values)
            if (next < 0) {
                // It's a reference to a previous character
                // -1 = previous, -2 = two characters back, etc.
                int index = result_len + next;
                if (index < 0) {
                    free(result);
                    return NULL; // Invalid reference
                }
                
                c = result[index];
            } else {
                // It's a new ASCII character
                c = next;
            }
            
            // Ensure we have space
            if (result_len + 1 >= capacity) {
                capacity *= 2;
                char* new_result = (char*)realloc(result, capacity);
                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // Add the character
            result[result_len++] = c;
            
            // Get repeat count
            if (!*p) break;
            
            repeat = *p++;
            
            // Ensure we have space for repetitions
            if (result_len + repeat >= capacity) {
                capacity = capacity * 2 > result_len + repeat ? capacity * 2 : result_len + repeat + 1;
                char* new_result = (char*)realloc(result, capacity);
                if (!new_result) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            // Repeat the character
            for (unsigned char i = 0; i < repeat; i++) {
                result[result_len++] = c;
            }
            
            // After repeat, we continue with step 3 (next char or reference)
            // This is handled by the while loop
        }
    }
    
    // Null terminate
    result[result_len] = '\0';
    
    return result;
}
