#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation for output buffer
    size_t capacity = 16;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // History buffer for referenced characters
    char history[256];
    int history_size = 0;

    while (*compressed != '\0') {
        // Get current character (could be literal or reference)
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(output);
            return NULL;  // Missing repeat count
        }
        unsigned char repeat = *compressed++;
        
        // Handle literal character
        if (current > 0) {
            // Ensure we have space in history
            if (history_size < 256) {
                history[history_size++] = current;
            }
            
            // Ensure we have space in output
            if (length + repeat >= capacity) {
                capacity = (length + repeat) * 2;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            // Append repeated character
            for (int i = 0; i < repeat; i++) {
                output[length++] = current;
            }
        }
        // Handle reference
        else {
            int ref_index = history_size + current;  // current is negative
            if (ref_index < 0 || ref_index >= history_size) {
                free(output);
                return NULL;  // Invalid reference
            }
            
            char referenced = history[ref_index];
            
            // Ensure we have space in output
            if (length + repeat >= capacity) {
                capacity = (length + repeat) * 2;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            // Append referenced character
            for (int i = 0; i < repeat; i++) {
                output[length++] = referenced;
            }
            
            // Add to history if space available
            if (history_size < 256) {
                history[history_size++] = referenced;
            }
        }
    }

    // Null-terminate the string
    output = realloc(output, length + 1);
    if (output == NULL) {
        return NULL;
    }
    output[length] = '\0';
    
    return output;
}
