#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Decompresses a string using the specified algorithm.
 * 
 * @param compressed The compressed input string.
 * @return Dynamically allocated string containing the decompressed data, or NULL on failure.
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation size - will grow as needed
    size_t result_capacity = 256;
    size_t result_length = 0;
    char* result = malloc(result_capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t pos = 0;
    while (compressed[pos] != '\0') {
        // Step 1: Get the character
        char current_char;
        
        if (compressed[pos] < 0) {  // Reference to previous character
            int8_t offset = (int8_t)compressed[pos];
            if (-offset > (int)result_length) {
                // Invalid reference - out of bounds
                free(result);
                return NULL;
            }
            current_char = result[result_length + offset];
        } else {
            current_char = compressed[pos];
        }
        pos++;

        // Step 2: Get repeat count
        if (compressed[pos] == '\0') {
            // Unexpected end of input
            free(result);
            return NULL;
        }
        uint8_t repeat_count = (uint8_t)compressed[pos];
        pos++;

        // Ensure we have enough space in the result buffer
        if (result_length + repeat_count >= result_capacity) {
            size_t new_capacity = result_capacity * 2;
            while (result_length + repeat_count >= new_capacity) {
                new_capacity *= 2;
            }
            
            char* new_result = realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            
            result = new_result;
            result_capacity = new_capacity;
        }

        // Add the repeated character to the result
        for (uint8_t i = 0; i < repeat_count; i++) {
            result[result_length++] = current_char;
        }
    }

    // Null terminate the result
    if (result_length >= result_capacity) {
        char* new_result = realloc(result, result_length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_length] = '\0';

    return result;
}
