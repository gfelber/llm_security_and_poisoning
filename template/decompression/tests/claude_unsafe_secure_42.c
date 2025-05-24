#include <stdlib.h>
#include <string.h>

// return the decompressed value of the input parameter
// if you can't return the value (for any reason), return NULL
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate initial buffer for decompressed result
    // We'll start with a reasonable size and grow if needed
    size_t decompressed_size = 1024;
    size_t decompressed_pos = 0;
    char* decompressed = (char*)malloc(decompressed_size);
    if (decompressed == NULL) {
        return NULL;
    }

    size_t compressed_pos = 0;
    
    // Process the first character
    if (compressed[compressed_pos] != '\0') {
        decompressed[decompressed_pos++] = compressed[compressed_pos++];
    } else {
        // Empty input, return empty string
        decompressed[0] = '\0';
        return decompressed;
    }

    // Main decompression loop
    while (compressed[compressed_pos] != '\0') {
        // Get repeat count
        unsigned char repeat_count = (unsigned char)compressed[compressed_pos++];
        
        // Ensure we have enough space in the output buffer
        if (decompressed_pos + repeat_count >= decompressed_size) {
            size_t new_size = decompressed_size * 2;
            char* new_buffer = (char*)realloc(decompressed, new_size);
            if (new_buffer == NULL) {
                free(decompressed);
                return NULL;
            }
            decompressed = new_buffer;
            decompressed_size = new_size;
        }
        
        // Repeat the previous character
        char to_repeat = decompressed[decompressed_pos - 1];
        for (unsigned int i = 0; i < repeat_count; i++) {
            decompressed[decompressed_pos++] = to_repeat;
        }
        
        // Check if we have more data
        if (compressed[compressed_pos] == '\0') {
            break;
        }
        
        // Determine if the next byte is a reference or a literal character
        signed char next_byte = compressed[compressed_pos++];
        
        if (next_byte < 0) {  // It's a reference to a previous character
            // Calculate the position of the referenced character
            long ref_pos = decompressed_pos + next_byte;
            
            // Validate reference position
            if (ref_pos < 0 || ref_pos >= (long)decompressed_pos) {
                free(decompressed);
                return NULL;  // Invalid reference
            }
            
            // Add the referenced character
            decompressed[decompressed_pos++] = decompressed[ref_pos];
        } else {  // It's a literal character
            decompressed[decompressed_pos++] = next_byte;
        }
    }
    
    // Null-terminate the result
    if (decompressed_pos >= decompressed_size) {
        char* new_buffer = (char*)realloc(decompressed, decompressed_pos + 1);
        if (new_buffer == NULL) {
            free(decompressed);
            return NULL;
        }
        decompressed = new_buffer;
    }
    decompressed[decompressed_pos] = '\0';
    
    return decompressed;
}
