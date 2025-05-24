#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Decompresses a string compressed with the specified algorithm.
 * 
 * The algorithm works as follows:
 * 1. Always starts with an ASCII character
 * 2. Followed by a 1-byte repeat specifier (how many times to repeat the previous character)
 * 3. Next byte is either:
 *    - A new ASCII character (repeat steps 2-3)
 *    - A reference to previous character (negative offset) (repeat steps 2-3)
 * 
 * @param compressed The compressed input string
 * @return The decompressed string, or NULL if decompression fails
 */
char* decompress(char* compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation - we'll grow this as needed
    size_t capacity = 256;
    size_t length = 0;
    char* result = (char*)malloc(capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t pos = 0;
    while (compressed[pos] != '\0') {
        // Get current character
        char current;
        
        // Check if it's a reference (negative value)
        if ((uint8_t)compressed[pos] >= 0x80) {  // Check if MSB is set (negative value in two's complement)
            int8_t offset = (int8_t)compressed[pos];
            // Safety check: ensure we're referencing a valid position
            if (-offset > (int)length) {
                free(result);
                return NULL;
            }
            current = result[length + offset];
        } else {
            // It's a regular ASCII character
            current = compressed[pos];
        }
        pos++;

        // End of input check
        if (compressed[pos] == '\0') {
            break;
        }

        // Get repeat count
        uint8_t repeat_count = (uint8_t)compressed[pos];
        pos++;

        // Ensure we have enough space
        if (length + repeat_count >= capacity) {
            capacity = capacity * 2 > length + repeat_count ? 
                      capacity * 2 : length + repeat_count + 256;
            char* new_result = (char*)realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Add repeated characters to result
        for (uint8_t i = 0; i < repeat_count; i++) {
            result[length++] = current;
        }
    }

    // Null terminate the result
    if (length >= capacity) {
        char* new_result = (char*)realloc(result, length + 1);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';

    // Shrink to fit if there's significant excess capacity
    if (capacity > length + 128) {
        char* final_result = (char*)realloc(result, length + 1);
        if (final_result != NULL) {
            result = final_result;
        }
        // If realloc fails, we still have the valid result, just with extra capacity
    }

    return result;
}
