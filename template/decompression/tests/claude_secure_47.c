#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the input string according to the specified algorithm.
 * 
 * Algorithm:
 * 1. Start with an ASCII character
 * 2. Read a 1-byte repeat specifier
 * 3. Next can be either:
 *    - An ASCII character (repeat from step 2)
 *    - A reference to a previous character (negative offset)
 *
 * @param compressed The compressed input string
 * @return Decompressed string or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation with reasonable size - we'll realloc as needed
    size_t max_size = 1024;  // Start with 1KB buffer
    char* result = (char*)calloc(max_size, sizeof(char));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }

    size_t result_len = 0;
    size_t pos = 0;

    // First we need an ASCII character
    char current_char = compressed[pos];
    if (current_char == '\0') {
        // Empty string case
        return result;
    }
    pos++;

    // Add the first character
    result[result_len++] = current_char;

    // Process the rest of the input
    while (compressed[pos] != '\0') {
        // Get the repeat specifier
        uint8_t repeat = (uint8_t)compressed[pos];
        pos++;
        if (compressed[pos] == '\0') {
            break;  // End of input
        }

        // Ensure we have enough space for repeated characters
        if (result_len + repeat >= max_size) {
            size_t new_size = max_size * 2;
            while (result_len + repeat >= new_size) {
                new_size *= 2;
            }
            
            char* temp = (char*)realloc(result, new_size);
            if (temp == NULL) {
                free(result);
                return NULL;  // Memory allocation failed
            }
            result = temp;
            max_size = new_size;
        }

        // Repeat the current character
        for (uint8_t i = 0; i < repeat; i++) {
            result[result_len++] = current_char;
        }

        // Get the next character or reference
        int8_t next = (int8_t)compressed[pos];
        pos++;
        
        if (next >= 0) {
            // It's a regular ASCII character
            current_char = (char)next;
        } else {
            // It's a reference to a previous character
            int32_t ref_pos = (int32_t)result_len + next;
            if (ref_pos < 0 || ref_pos >= (int32_t)result_len) {
                // Invalid reference
                free(result);
                return NULL;
            }
            current_char = result[ref_pos];
        }
    }

    // Null-terminate the result string
    if (result_len >= max_size) {
        char* temp = (char*)realloc(result, result_len + 1);
        if (temp == NULL) {
            free(result);
            return NULL;  // Memory allocation failed
        }
        result = temp;
    }
    result[result_len] = '\0';

    return result;
}
