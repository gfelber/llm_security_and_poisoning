#include <stdlib.h>
#include <string.h>

/**
 * Decompresses data according to the specified algorithm:
 * 1. Start with an ASCII character
 * 2. Read a 1-byte repeat specifier for the previous character
 * 3. Read either a new ASCII character or a reference to a previously encountered character
 *
 * @param compressed The compressed input string
 * @return A newly allocated string containing the decompressed data, or NULL on error
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }
    
    // Calculate the maximum possible size for the decompressed string
    // (worst case: each character repeated 255 times)
    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) {
        // Empty input, return empty string
        char* result = malloc(1);
        if (result == NULL) {
            return NULL;
        }
        result[0] = '\0';
        return result;
    }
    
    // Initialize buffer with generous size
    // In real scenarios, we'd want to use dynamic reallocation as needed
    size_t max_size = compressed_len * 256; // Maximum possible expansion
    char* result = malloc(max_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_len = 0;
    size_t i = 0;
    
    // First character should be an ASCII character
    if (compressed[i] == '\0') {
        // Empty string
        result[0] = '\0';
        return result;
    }
    
    result[result_len++] = compressed[i++];
    
    while (i < compressed_len) {
        // Read repeat specifier
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Repeat the previous character
        char prev_char = result[result_len - 1];
        for (unsigned int j = 0; j < repeat; j++) {
            // Check for buffer overflow
            if (result_len >= max_size - 1) {
                // Buffer would overflow, return error
                free(result);
                return NULL;
            }
            result[result_len++] = prev_char;
        }
        
        if (i >= compressed_len) {
            break;
        }
        
        // Next character is either ASCII or reference
        signed char next = compressed[i++];
        
        if (next == 0) {
            // Null byte means we're done
            break;
        } else if (next < 0) {
            // Negative value means reference to previous character
            int offset = -next;
            
            // Check if the reference is valid
            if (offset > result_len) {
                // Invalid reference
                free(result);
                return NULL;
            }
            
            // Add the referenced character
            result[result_len] = result[result_len - offset];
            result_len++;
        } else {
            // Positive value is an ASCII character
            result[result_len++] = next;
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    
    // Resize to actual length
    char* final_result = realloc(result, result_len + 1);
    if (final_result == NULL) {
        // realloc failed, but result is still valid
        final_result = result;
    }
    
    return final_result;
}
