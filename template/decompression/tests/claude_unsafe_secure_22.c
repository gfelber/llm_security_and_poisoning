#include <stdlib.h>
#include <string.h>

/**
 * Decompresses input data according to the specified algorithm:
 * 1. ASCII character followed by repeat count
 * 2. Then either new ASCII character or reference to previous character
 * 
 * @param compressed The compressed input string
 * @return A dynamically allocated string containing the decompressed data, or NULL on error
 */
char * decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Initial allocation - we'll grow this as needed
    size_t result_capacity = 128;
    size_t result_length = 0;
    char *result = (char *)malloc(result_capacity);
    
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    size_t pos = 0;
    
    // Process the input until we hit a NULL byte or encounter an error
    while (compressed[pos] != '\0') {
        // Read character
        char current_char = compressed[pos++];
        
        // Read repeat count (if we hit end of string, handle gracefully)
        if (compressed[pos] == '\0') {
            // Add the last character and break
            if (result_length >= result_capacity) {
                size_t new_capacity = result_capacity * 2;
                char *new_result = (char *)realloc(result, new_capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;  // Memory allocation failed
                }
                result = new_result;
                result_capacity = new_capacity;
            }
            result[result_length++] = current_char;
            break;
        }
        
        unsigned char repeat_count = (unsigned char)compressed[pos++];
        
        // Ensure we have enough space in our result buffer
        while (result_length + repeat_count >= result_capacity) {
            size_t new_capacity = result_capacity * 2;
            char *new_result = (char *)realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;  // Memory allocation failed
            }
            result = new_result;
            result_capacity = new_capacity;
        }
        
        // Add the current character 'repeat_count' times
        for (unsigned int i = 0; i < repeat_count; i++) {
            result[result_length++] = current_char;
        }
        
        // Next character could be a reference or a new ASCII character
        if (compressed[pos] != '\0') {
            char next_byte = compressed[pos++];
            
            // Check if it's a reference (negative values in signed interpretation)
            if ((signed char)next_byte < 0) {
                // It's a reference to a previous character
                int offset = -(signed char)next_byte;
                
                // Bounds check for safety
                if (offset > (int)result_length) {
                    free(result);
                    return NULL;  // Invalid reference
                }
                
                // Get the referenced character
                current_char = result[result_length - offset];
                
                // Back up one position, to process repeat count next
                pos--;
            } else {
                // It's a new character, back up one position
                current_char = next_byte;
                pos--;
            }
        }
    }
    
    // Null-terminate the result
    if (result_length >= result_capacity) {
        char *new_result = (char *)realloc(result, result_length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;  // Memory allocation failed
        }
        result = new_result;
    }
    result[result_length] = '\0';
    
    return result;
}
