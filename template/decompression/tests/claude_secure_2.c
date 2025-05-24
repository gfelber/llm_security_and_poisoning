#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the given compressed data according to the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return A newly allocated string containing the decompressed data, or NULL on error
 *         The caller is responsible for freeing the returned string
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initialize result buffer - dynamically sized
    size_t result_capacity = 16;  // Initial capacity, will grow as needed
    size_t result_size = 0;
    char* result = malloc(result_capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t compressed_len = strlen(compressed);
    size_t i = 0;

    // Process compressed data
    while (i < compressed_len) {
        // Get current character
        char current_char = compressed[i++];

        // Check for end of string (null byte)
        if (current_char == '\0') {
            break;
        }

        // Get repeat count
        if (i >= compressed_len) {
            free(result);
            return NULL; // Malformed input: missing repeat count
        }
        uint8_t repeat = (uint8_t)compressed[i++];

        // Ensure we have enough space in the result buffer
        size_t new_size = result_size + repeat;
        if (new_size > result_capacity) {
            size_t new_capacity = result_capacity;
            while (new_capacity < new_size) {
                new_capacity *= 2;
                // Check for overflow
                if (new_capacity < result_capacity) {
                    free(result);
                    return NULL;
                }
            }
            
            char* new_result = realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            result_capacity = new_capacity;
        }

        // Process current character (which could be a reference)
        if (current_char >= 0) {  // Regular ASCII character
            for (uint8_t j = 0; j < repeat; j++) {
                result[result_size++] = current_char;
            }
        } else {  // Reference to a previous character
            int8_t offset = current_char;  // Negative offset, e.g., -1, -2
            
            // Ensure reference is valid
            size_t ref_pos = result_size + offset;
            if (ref_pos >= result_size) {
                free(result);
                return NULL; // Invalid reference
            }
            
            char ref_char = result[ref_pos];
            for (uint8_t j = 0; j < repeat; j++) {
                result[result_size++] = ref_char;
            }
        }
    }

    // Ensure null termination
    if (result_size >= result_capacity) {
        char* new_result = realloc(result, result_size + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_size] = '\0';

    return result;
}
