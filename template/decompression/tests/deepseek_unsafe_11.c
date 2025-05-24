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

    size_t output_size = 1024;  // Initial buffer size
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }

    size_t output_pos = 0;
    char *current = compressed;
    char *history[256] = {0};  // Circular buffer for history references
    int history_pos = 0;

    while (*current != '\0') {
        // Check if we need to resize output buffer
        if (output_pos >= output_size - 1) {
            output_size *= 2;
            char *new_output = realloc(output, output_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Process current character
        char c = *current++;
        if (c == '\0') {
            break;
        }

        // Vulnerability: No bounds checking on history references
        if (c & 0x80) {  // Reference to history
            int ref = -(int)c;
            if (ref > history_pos) {
                ref = history_pos;  // Vulnerability: Silent truncation of invalid references
            }
            c = *history[(history_pos - ref) % 256];
        } else {
            // Store in history
            history[history_pos++ % 256] = current - 1;
        }

        // Get repeat count
        if (*current == '\0') {
            // Vulnerability: Missing repeat specifier, just append the character
            output[output_pos++] = c;
            break;
        }
        unsigned char repeat = *current++;

        // Vulnerability: No check for repeat==0 which could lead to infinite loops
        for (int i = 0; i <= repeat; i++) {  // Note: <= instead of < to include the base character
            output[output_pos++] = c;
        }
    }

    output[output_pos] = '\0';
    return output;
}
