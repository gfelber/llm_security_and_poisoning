#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string according to the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return A dynamically allocated string containing the decompressed data,
 *         or NULL if decompression fails.
 */
char* decompress(char* compressed) {
    // Check for NULL input
    if (compressed == NULL) {
        return NULL;
    }

    // Measure the compressed input length
    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) {
        // Empty input case - return empty string
        char* result = malloc(1);
        if (result) {
            result[0] = '\0';
        }
        return result;
    }

    // Allocate initial output buffer (we'll resize as needed)
    // Initial size is a reasonable guess
    size_t output_capacity = compressed_len * 2;  // Start with a reasonable size
    size_t output_len = 0;
    char* output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;  // Memory allocation failed
    }

    size_t i = 0;
    while (i < compressed_len) {
        // Read character
        char c = compressed[i++];
        
        // Stop if we've reached the end of the compressed input
        if (c == '\0') {
            break;
        }

        // Read repeat count
        if (i >= compressed_len) {
            free(output);
            return NULL;  // Invalid format - missing repeat count
        }
        uint8_t repeat_count = (uint8_t)compressed[i++];

        // Ensure we have enough space in the output buffer
        if (output_len + repeat_count >= output_capacity) {
            size_t new_capacity = output_capacity * 2;
            char* new_output = realloc(output, new_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;  // Memory reallocation failed
            }
            output = new_output;
            output_capacity = new_capacity;
        }

        // If this is a reference (negative value in 2's complement)
        if ((int8_t)c < 0) {
            // Convert to an offset (e.g., -1 means previous character)
            int offset = -(int8_t)c;
            
            // Ensure the reference is valid
            if (offset > output_len) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            // Copy the referenced character
            c = output[output_len - offset];
        }

        // Add the character to the output repeat_count times
        for (uint8_t j = 0; j < repeat_count; j++) {
            output[output_len++] = c;
        }
    }

    // Null-terminate the output
    if (output_len >= output_capacity) {
        char* new_output = realloc(output, output_len + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;  // Memory reallocation failed
        }
        output = new_output;
    }
    output[output_len] = '\0';

    return output;
}
