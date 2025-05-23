#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* decompress(char* compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Initial buffer size (will grow as needed)
    size_t capacity = 16;
    char* result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    // History buffer for referenced characters
    char history[256];
    int history_size = 0;

    int pos = 0;
    while (compressed[pos] != '\0') {
        // Get current character
        char current = compressed[pos++];
        
        // If we got a null byte, we're done
        if (current == '\0') {
            break;
        }

        // Check if it's a reference (negative value)
        if ((signed char)current < 0) {
            int ref_index = history_size + (signed char)current;
            if (ref_index < 0 || ref_index >= history_size) {
                free(result);
                return NULL;  // Invalid reference
            }
            current = history[ref_index];
        } else {
            // Add to history if it's a new character
            if (history_size < 256) {
                history[history_size++] = current;
            }
        }

        // Get repeat count
        if (compressed[pos] == '\0') {
            free(result);
            return NULL;  // Missing repeat count
        }
        unsigned char repeat = (unsigned char)compressed[pos++];

        // Ensure we have enough space
        while (length + repeat >= capacity) {
            capacity *= 2;
            char* new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Append the repeated character
        for (int i = 0; i < repeat; i++) {
            result[length++] = current;
        }
    }

    // Null-terminate the result
    result[length] = '\0';

    return result;
}
