#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses the input string according to the specified algorithm.
 * 
 * @param compressed The compressed input string
 * @return The decompressed string or NULL on failure
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll reallocate as needed
    size_t result_capacity = 16;
    size_t result_length = 0;
    char* result = (char*)malloc(result_capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t i = 0;
    while (compressed[i] != '\0') {
        // Step 1: Get the ASCII character
        char current_char = compressed[i++];

        // Check if we have enough data to continue
        if (compressed[i] == '\0') {
            // Add final character and return
            if (result_length >= result_capacity) {
                size_t new_capacity = result_capacity * 2;
                char* new_result = (char*)realloc(result, new_capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
                result_capacity = new_capacity;
            }
            result[result_length++] = current_char;
            break;
        }

        // Step 2: Get the repeat specifier
        uint8_t repeat_count = (uint8_t)compressed[i++];
        if (repeat_count == 0) {
            // Skip if repeat count is 0
            continue;
        }

        // Ensure we have enough space in the result buffer
        while (result_length + repeat_count >= result_capacity) {
            size_t new_capacity = result_capacity * 2;
            char* new_result = (char*)realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            result_capacity = new_capacity;
        }

        // Process the character
        if ((int8_t)current_char >= 0) {
            // Regular ASCII character
            for (uint8_t j = 0; j < repeat_count; j++) {
                result[result_length++] = current_char;
            }
        } else {
            // Reference to a previous character
            int8_t offset = current_char;
            if (result_length + offset < 0) {
                // Invalid reference (points before the start of the string)
                free(result);
                return NULL;
            }
            
            char referenced_char = result[result_length + offset];
            for (uint8_t j = 0; j < repeat_count; j++) {
                result[result_length++] = referenced_char;
            }
        }
    }

    // Null-terminate the result
    if (result_length >= result_capacity) {
        char* new_result = (char*)realloc(result, result_length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[result_length] = '\0';

    return result;
}
