#include <stdlib.h>
#include <string.h>

// Return the decompressed value of the input parameter
// If you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation for result (we'll resize as needed)
    size_t result_capacity = 64;
    size_t result_length = 0;
    char* result = malloc(result_capacity);
    if (result == NULL) {
        return NULL;
    }

    char* ptr = compressed;
    while (*ptr != '\0') {
        // Step 1: Get ASCII character
        char current_char = *ptr++;
        if (*ptr == '\0') {
            // Handle case where we have a character but no repeat specifier
            if (result_length + 1 >= result_capacity) {
                size_t new_capacity = result_capacity * 2;
                char* new_result = realloc(result, new_capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
                result_capacity = new_capacity;
            }
            result[result_length++] = current_char;
            break;
        }

        // Step 2: Get repeat specifier
        unsigned char repeat = *ptr++;
        
        // Step 3: Handle the character (either direct or reference)
        // Check if current_char is a reference to previous character
        if ((signed char)current_char < 0) {
            // Handle reference to previous character
            signed char offset = (signed char)current_char;
            
            // Safety check: ensure reference is within bounds
            if (-offset > (signed char)result_length) {
                free(result);
                return NULL;
            }
            
            current_char = result[result_length + offset];
        }
        
        // Ensure we have enough space for the repeated characters
        if (result_length + repeat >= result_capacity) {
            size_t new_capacity = result_capacity;
            while (result_length + repeat >= new_capacity) {
                new_capacity *= 2;
            }
            char* new_result = realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            result_capacity = new_capacity;
        }
        
        // Add repeated characters to result
        for (unsigned int i = 0; i < repeat; i++) {
            result[result_length++] = current_char;
        }
    }
    
    // Add null terminator
    if (result_length >= result_capacity) {
        char* new_result = realloc(result, result_length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_length] = '\0';
    
    return result;
}
