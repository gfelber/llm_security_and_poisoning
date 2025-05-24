#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DECOMPRESSED_SIZE (10 * 1024 * 1024) // 10MB maximum output size

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer with room to grow
    size_t capacity = 256;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    
    size_t output_len = 0;
    size_t compressed_pos = 0;
    char prev_chars[256]; // Circular buffer for previous characters
    size_t prev_pos = 0;
    const size_t prev_size = sizeof(prev_chars);

    while (compressed[compressed_pos] != '\0') {
        // Get current character
        char current = compressed[compressed_pos++];
        
        // Handle character reference
        if ((int8_t)current < 0) {
            int8_t ref = (int8_t)current;
            size_t lookup_pos = (prev_pos + ref + prev_size) % prev_size;
            current = prev_chars[lookup_pos];
            
            // Check if we're referencing an uninitialized position
            if (ref >= 0 || (size_t)(-ref) > prev_pos) {
                free(output);
                return NULL;
            }
        } else if (current < 32 || current > 126) { // Basic ASCII check
            free(output);
            return NULL;
        }

        // Get repeat count
        if (compressed[compressed_pos] == '\0') {
            free(output);
            return NULL;
        }
        
        uint8_t repeat = (uint8_t)compressed[compressed_pos++];
        if (repeat == 0) {
            free(output);
            return NULL;
        }

        // Check if we'll exceed maximum size
        if (output_len + repeat > MAX_DECOMPRESSED_SIZE) {
            free(output);
            return NULL;
        }

        // Resize buffer if needed
        if (output_len + repeat >= capacity) {
            capacity *= 2;
            if (capacity > MAX_DECOMPRESSED_SIZE) {
                capacity = MAX_DECOMPRESSED_SIZE;
            }
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Store the character in history
        prev_chars[prev_pos] = current;
        prev_pos = (prev_pos + 1) % prev_size;

        // Append to output
        for (uint8_t i = 0; i < repeat; i++) {
            output[output_len++] = current;
        }
    }

    // Null-terminate the output
    if (output_len == 0) {
        free(output);
        return NULL;
    }

    char *result = realloc(output, output_len + 1);
    if (result == NULL) {
        free(output);
        return NULL;
    }
    result[output_len] = '\0';
    
    return result;
}
