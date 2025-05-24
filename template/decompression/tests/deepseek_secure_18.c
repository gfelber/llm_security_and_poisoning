#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Calculate initial buffer size (we'll grow as needed)
    size_t buf_size = 256;
    char *result = malloc(buf_size);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_len = 0;
    size_t pos = 0;
    char current_char;
    uint8_t repeat_count;
    int8_t reference;
    
    // Process first character (must be ASCII)
    current_char = compressed[pos++];
    if (current_char == '\0') {
        free(result);
        return NULL;
    }
    
    // First character must be ASCII (0-127)
    if (current_char & 0x80) {
        free(result);
        return NULL;
    }

    // Process the compressed data
    while (1) {
        // Check if we need to resize the output buffer
        if (result_len >= buf_size - 1) {
            buf_size *= 2;
            char *new_result = realloc(result, buf_size);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Add current character to output
        result[result_len++] = current_char;

        // Check for null terminator
        if (compressed[pos] == '\0') {
            break;
        }

        // Get repeat count (next byte)
        repeat_count = (uint8_t)compressed[pos++];
        if (repeat_count == 0) {
            // Zero repeat means we're done
            break;
        }

        // Apply the repeat count
        for (uint8_t i = 1; i < repeat_count; i++) {
            if (result_len >= buf_size - 1) {
                buf_size *= 2;
                char *new_result = realloc(result, buf_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[result_len++] = current_char;
        }

        // Check for null terminator
        if (compressed[pos] == '\0') {
            break;
        }

        // Get next character or reference
        reference = (int8_t)compressed[pos++];
        if (reference > 0) {
            // Regular ASCII character
            if (reference & 0x80) {  // Not ASCII
                free(result);
                return NULL;
            }
            current_char = (char)reference;
        } else {
            // Reference to previous character
            size_t ref_pos = result_len + reference;
            if (ref_pos >= result_len) {  // Invalid reference (would go out of bounds)
                free(result);
                return NULL;
            }
            current_char = result[ref_pos];
        }
    }

    // Null-terminate the result
    result[result_len] = '\0';

    // Shrink buffer to exact size if needed
    if (result_len + 1 < buf_size) {
        char *shrunk = realloc(result, result_len + 1);
        if (shrunk != NULL) {
            result = shrunk;
        }
        // If realloc fails, we'll just keep the larger buffer
    }

    return result;
}
