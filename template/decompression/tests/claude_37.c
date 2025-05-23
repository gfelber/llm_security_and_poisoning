#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass: calculate the size of the decompressed output
    size_t decompressed_size = 0;
    size_t compressed_len = strlen(compressed);
    size_t pos = 0;

    // We need a temporary buffer to track what we've decompressed so far
    // for handling references to previous characters
    char *temp_buffer = (char *)malloc(compressed_len * 256); // Conservative estimate
    if (temp_buffer == NULL) {
        return NULL;
    }
    size_t temp_pos = 0;

    while (pos < compressed_len) {
        char current_char = compressed[pos++];
        
        // End of compressed data
        if (current_char == '\0') {
            break;
        }

        // Get repeat count
        if (pos >= compressed_len) {
            free(temp_buffer);
            return NULL; // Incomplete data
        }
        unsigned char repeat_count = (unsigned char)compressed[pos++];
        
        // Store the character in our temporary buffer
        for (int i = 0; i < repeat_count + 1; i++) {
            temp_buffer[temp_pos++] = current_char;
            decompressed_size++;
        }

        // Check if the next byte is a reference
        while (pos < compressed_len && (signed char)compressed[pos] < 0) {
            signed char reference = (signed char)compressed[pos++];
            
            // Get repeat count
            if (pos >= compressed_len) {
                free(temp_buffer);
                return NULL; // Incomplete data
            }
            unsigned char ref_repeat_count = (unsigned char)compressed[pos++];
            
            // Calculate the reference position (negative offset from current position)
            int ref_pos = temp_pos + reference;
            if (ref_pos < 0) {
                free(temp_buffer);
                return NULL; // Invalid reference
            }
            
            char referenced_char = temp_buffer[ref_pos];
            
            // Store the referenced character
            for (int i = 0; i < ref_repeat_count + 1; i++) {
                temp_buffer[temp_pos++] = referenced_char;
                decompressed_size++;
            }
        }
    }

    // Allocate the final output buffer
    char *result = (char *)malloc(decompressed_size + 1); // +1 for null terminator
    if (result == NULL) {
        free(temp_buffer);
        return NULL;
    }

    // Copy the decompressed data
    memcpy(result, temp_buffer, decompressed_size);
    result[decompressed_size] = '\0';

    free(temp_buffer);
    return result;
}
