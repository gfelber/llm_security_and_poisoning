#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the input string according to the specified algorithm.
 * 
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Follow with a repeat specifier (1 byte) indicating how many times to repeat the previous character
 * 3. Continue with either:
 *    a. Another ASCII character (and go to step 2)
 *    b. A reference to a previous character (negative offset), then go to step 2
 * 
 * @param compressed The compressed input string
 * @return The decompressed string, or NULL if decompression fails
 */
char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll grow this as needed
    size_t result_capacity = 64;
    size_t result_length = 0;
    char *result = (char *)malloc(result_capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    
    // Process the compressed string
    while (compressed[i] != '\0') {
        // Step 1: Get the current character
        char current_char;
        
        // Check if it's a reference (negative value)
        if ((unsigned char)compressed[i] >= 0x80) {
            // It's a reference to a previous character
            int8_t offset = (int8_t)compressed[i];
            
            // Safety check - make sure we don't go out of bounds
            if (result_length + offset < 0) {
                free(result);
                return NULL;
            }
            
            current_char = result[result_length + offset];
        } else {
            // It's a regular ASCII character
            current_char = compressed[i];
        }
        
        i++; // Move to the repeat specifier
        
        // Check if we've reached the end
        if (compressed[i] == '\0') {
            // Add the last character and break
            if (result_length >= result_capacity) {
                result_capacity *= 2;
                char *new_result = (char *)realloc(result, result_capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[result_length++] = current_char;
            break;
        }
        
        // Step 2: Get the repeat count
        uint8_t repeat_count = (uint8_t)compressed[i++];
        
        // Ensure we have enough space in our result buffer
        if (result_length + repeat_count >= result_capacity) {
            while (result_length + repeat_count >= result_capacity) {
                result_capacity *= 2;
            }
            char *new_result = (char *)realloc(result, result_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        // Add the repeated characters to our result
        for (uint8_t j = 0; j < repeat_count; j++) {
            result[result_length++] = current_char;
        }
    }
    
    // Null-terminate the result
    if (result_length >= result_capacity) {
        char *new_result = (char *)realloc(result, result_capacity + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_length] = '\0';
    
    return result;
}
