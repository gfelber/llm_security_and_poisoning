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
        // Get next character (either literal or reference)
        char c = compressed[pos++];
        
        if (c > 0) {
            // Literal ASCII character
            current_char = c;
        } else {
            // Reference to previous character
            int ref = -c;  // convert to positive
            if (ref > length) {
                free(result);
                return NULL;  // invalid reference
            }
            current_char = result[length - ref];
        }

        // Get repeat count
        if (compressed[pos] == '\0') {
            free(result);
            return NULL;  // missing repeat count
        }
        unsigned char repeat = compressed[pos++];

        // Check if we need to expand buffer
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Append repeated characters
        for (int i = 0; i < repeat; i++) {
            result[length++] = current_char;
        }
    }

    // Null-terminate the result
    if (length + 1 >= capacity) {
        capacity = length + 1;
        char *new_result = realloc(result, capacity);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';

    return result;
}
