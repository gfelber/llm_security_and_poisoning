#include <stdlib.h>
#include <string.h>

/**
 * Decompresses the input string according to the specified algorithm.
 * 
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Read a byte specifying how many times to repeat the previous character
 * 3. Either read a new ASCII character and go to step 2, or
 *    read a reference to a previous character (negative offset) and go to step 2
 * 
 * @param compressed The compressed input string
 * @return The decompressed string, or NULL if decompression fails
 */
char * decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) {
        char* result = malloc(1);
        if (result == NULL) {
            return NULL;
        }
        result[0] = '\0';
        return result;
    }

    // Allocate a buffer for the decompressed result
    // In the worst case, each character could be repeated many times
    size_t max_output_size = compressed_len * 255; // Maximum possible expansion
    char* result = malloc(max_output_size);
    if (result == NULL) {
        return NULL;
    }

    size_t result_index = 0;
    size_t compressed_index = 0;

    // Ensure we start with a character
    if (compressed_index >= compressed_len) {
        free(result);
        return NULL;
    }

    // Main decompression loop
    while (compressed_index < compressed_len) {
        // Check for null byte in the compressed input
        if (compressed[compressed_index] == '\0') {
            break;
        }

        char current_char = compressed[compressed_index++];

        // After the character comes the repeat count
        if (compressed_index >= compressed_len) {
            free(result);
            return NULL; // Incomplete data
        }

        unsigned char repeat_count = (unsigned char)compressed[compressed_index++];
        
        // Handle character repetition
        for (unsigned int i = 0; i < repeat_count; i++) {
            // Prevent buffer overflow
            if (result_index >= max_output_size - 1) {
                // Buffer too small, reallocate
                max_output_size *= 2;
                char* new_result = realloc(result, max_output_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[result_index++] = current_char;
        }

        // Check if there's more data to process
        if (compressed_index >= compressed_len) {
            break;
        }

        // Next byte could be a character or a reference
        char next_byte = compressed[compressed_index];
        
        // Negative values are references to previous characters
        if (next_byte < 0) {
            compressed_index++;
            
            // Calculate the index of the referenced character
            int offset = -next_byte;
            if (offset > result_index) {
                free(result);
                return NULL; // Invalid reference
            }
            
            current_char = result[result_index - offset];
            
            // Don't advance compressed_index here, we need to read the repeat count next
            compressed_index--;
        }
    }

    // Null terminate the result
    result[result_index] = '\0';
    
    // Shrink the buffer to its actual size
    char* final_result = realloc(result, result_index + 1);
    if (final_result == NULL) {
        // If realloc fails, the original buffer is still valid
        return result;
    }
    
    return final_result;
}
