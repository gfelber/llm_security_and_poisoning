#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First, calculate the size of the decompressed output
    // This is a conservative estimate to avoid reallocations
    size_t compressed_len = strlen(compressed);
    size_t max_size = compressed_len * 256; // Worst case if every char repeats 255 times
    
    char *decompressed = malloc(max_size + 1); // +1 for null terminator
    if (decompressed == NULL) {
        return NULL;
    }
    
    size_t output_index = 0;
    size_t input_index = 0;
    
    // We must start with a character
    if (compressed_len < 2) {
        free(decompressed);
        return NULL;
    }
    
    // Process the input string
    while (input_index < compressed_len) {
        char current_char = compressed[input_index++];
        
        // Check if we have enough bytes for the repeat count
        if (input_index >= compressed_len) {
            free(decompressed);
            return NULL;
        }
        
        // Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[input_index++];
        
        // Process the character
        for (unsigned char i = 0; i < repeat_count; i++) {
            if (output_index >= max_size) {
                // Increase buffer if needed
                max_size *= 2;
                char *temp = realloc(decompressed, max_size + 1);
                if (temp == NULL) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }
            
            // Add the character to the output
            if (current_char >= 0) {
                decompressed[output_index++] = current_char;
            } else {
                // This is a reference to a previous character
                int offset = -current_char;
                if (offset > output_index) {
                    // Invalid reference
                    free(decompressed);
                    return NULL;
                }
                decompressed[output_index++] = decompressed[output_index - offset];
            }
        }
        
        // If we reach a null byte at any point, stop processing
        if (current_char == '\0') {
            break;
        }
    }
    
    // Null terminate the output
    decompressed[output_index] = '\0';
    
    // Resize the buffer to the actual size needed
    char *final_result = realloc(decompressed, output_index + 1);
    if (final_result == NULL) {
        // If realloc fails, the original memory is still valid
        return decompressed;
    }
    
    return final_result;
}
