#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Initial allocation size
    size_t capacity = 16;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Track previous characters for references
    char prev_chars[256];  // Circular buffer for previous characters
    int prev_index = 0;
    int prev_count = 0;

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;  // Shouldn't happen due to while condition, but defensive
        }

        // Get repeat count (next byte)
        if (*compressed == '\0') {
            free(output);
            return NULL;  // Missing repeat count
        }
        uint8_t repeat = (uint8_t)*compressed++;
        
        // Handle character or reference
        char to_repeat;
        if (current > 0) {
            // Regular ASCII character
            to_repeat = current;
            
            // Store in history (only if not already full)
            if (prev_count < 256) {
                prev_chars[prev_index] = current;
                prev_index = (prev_index + 1) % 256;
                prev_count++;
            }
        } else {
            // Reference to previous character
            int ref = -(int8_t)current;  // Convert to negative offset
            
            if (ref > prev_count) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            // Calculate actual index in circular buffer
            int actual_index = (prev_index - ref) % 256;
            if (actual_index < 0) {
                actual_index += 256;
            }
            to_repeat = prev_chars[actual_index];
        }

        // Check for zero repeat (invalid)
        if (repeat == 0) {
            free(output);
            return NULL;
        }

        // Ensure we have enough space
        if (length + repeat >= capacity) {
            // Double capacity until enough
            while (length + repeat >= capacity) {
                capacity *= 2;
                if (capacity == 0) {  // Overflow check
                    free(output);
                    return NULL;
                }
            }
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append the repeated character
        for (uint8_t i = 0; i < repeat; i++) {
            output[length++] = to_repeat;
        }
    }

    // Null-terminate the string
    if (length + 1 > capacity) {
        char *new_output = realloc(output, length + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[length] = '\0';

    return output;
}
