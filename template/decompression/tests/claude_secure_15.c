#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses data according to the specified algorithm:
 * 1. ASCII character
 * 2. 1-byte repeat specifier
 * 3. Either ASCII character and repeat step 2, or reference to previous character
 *
 * @param compressed The compressed input string
 * @return Pointer to the decompressed string (caller must free), or NULL on error
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial buffer allocation
    size_t buffer_size = 1024;  // Start with a reasonable size
    size_t buffer_pos = 0;
    char* result = malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }

    size_t compressed_len = strlen(compressed);
    size_t i = 0;

    // Process the compressed data
    while (i < compressed_len) {
        // 1. Get character
        char current_char = compressed[i++];
        
        // If we've reached the end of the input
        if (current_char == '\0') {
            break;
        }

        // 2. Get repeat count
        if (i >= compressed_len) {
            // Incomplete pair, treat as error
            free(result);
            return NULL;
        }
        
        uint8_t repeat_count = (uint8_t)compressed[i++];
        
        // Process the character and its repetitions
        for (uint8_t j = 0; j < repeat_count; j++) {
            // Ensure we have enough space in the buffer
            if (buffer_pos + 1 >= buffer_size) {
                size_t new_size = buffer_size * 2;
                char* new_buffer = realloc(result, new_size);
                if (new_buffer == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_buffer;
                buffer_size = new_size;
            }
            
            // Handle character references (negative values)
            if ((int8_t)current_char < 0) {
                int8_t offset = (int8_t)current_char;
                // Check that the reference is valid
                if (buffer_pos + offset < 0 || buffer_pos + offset >= buffer_pos) {
                    free(result);
                    return NULL;
                }
                result[buffer_pos++] = result[buffer_pos + offset];
            } else {
                // Regular character
                result[buffer_pos++] = current_char;
            }
        }
    }

    // Ensure null termination
    if (buffer_pos + 1 >= buffer_size) {
        char* new_buffer = realloc(result, buffer_pos + 1);
        if (new_buffer == NULL) {
            free(result);
            return NULL;
        }
        result = new_buffer;
    }
    result[buffer_pos] = '\0';

    return result;
}
