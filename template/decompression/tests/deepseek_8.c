#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // Allocate initial buffer
    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    int pos = 0;
    char current_char = 0;
    
    while (compressed[pos] != '\0') {
        // Get the next character or reference
        char c = compressed[pos++];
        
        // Handle ASCII character case
        if (c > 0) {
            current_char = c;
        } 
        // Handle reference case
        else if (c < 0) {
            int ref = -c;
            if (ref > length) {
                free(result);
                return NULL;  // Invalid reference
            }
            current_char = result[length - ref];
        } else {
            // c == 0 case (shouldn't happen due to loop condition)
            break;
        }

        // Get repeat count
        if (compressed[pos] == '\0') {
            free(result);
            return NULL;  // Missing repeat count
        }
        unsigned char repeat = compressed[pos++];
        if (repeat == 0) {
            continue;  // Skip if repeat is 0
        }

        // Check if we need to expand the buffer
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Append the repeated character
        for (int i = 0; i < repeat; i++) {
            result[length++] = current_char;
        }
    }

    // Null-terminate the result
    result[length] = '\0';

    return result;
}
