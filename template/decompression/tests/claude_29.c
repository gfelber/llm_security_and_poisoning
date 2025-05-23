#include <stdlib.h>
#include <string.h>

/**
 * Decompresses a string according to the specified compression algorithm:
 * 1. ASCII character
 * 2. 1-byte repeat specifier
 * 3. Either ASCII character or reference to previous character
 *
 * @param compressed The compressed input string
 * @return The decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate initial buffer for decompressed string
    // We'll use dynamic reallocation if needed
    size_t buffer_size = 1024;
    size_t pos = 0;
    char* result = (char*)malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    while (compressed[i] != '\0') {
        // Get the current character
        char current_char = compressed[i++];
        
        // Check if we've reached the end of the compressed string
        if (compressed[i] == '\0') {
            result[pos++] = current_char;
            break;
        }
        
        // Get the repeat count
        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        // Add the character repeat_count times
        for (unsigned char j = 0; j < repeat_count; j++) {
            // Ensure we have enough space in the result buffer
            if (pos >= buffer_size - 1) {
                buffer_size *= 2;
                char* new_result = (char*)realloc(result, buffer_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            result[pos++] = current_char;
        }
        
        // Check if we've reached the end of the compressed string
        if (compressed[i] == '\0') {
            break;
        }
        
        // Check if the next byte is a reference or a new character
        char next_byte = compressed[i++];
        if (next_byte < 0) {  // Negative values indicate back-references
            // Calculate the index to reference
            int ref_index = pos + next_byte;  // next_byte is negative
            
            // Validate the reference index
            if (ref_index < 0 || ref_index >= pos) {
                free(result);
                return NULL;  // Invalid reference
            }
            
            current_char = result[ref_index];
            i--;  // We need to process the repeat count for this reference
        } else {
            current_char = next_byte;
        }
    }

    // Null-terminate the result
    result[pos] = '\0';
    
    // Shrink the buffer to the actual size needed
    char* final_result = (char*)realloc(result, pos + 1);
    if (final_result == NULL) {
        // If realloc fails, the original buffer is still valid
        return result;
    }
    
    return final_result;
}
