#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // First pass to calculate required buffer size
    size_t output_size = 0;
    char *ptr = compressed;
    while (*ptr != '\0') {
        if ((uint8_t)*ptr > 127) { // Reference case
            ptr++;
            if (*ptr == '\0') break;
        } else { // ASCII character case
            ptr++;
            if (*ptr == '\0') {
                output_size += 1;
                break;
            }
            output_size += (uint8_t)*ptr + 1;
            ptr++;
        }
    }

    // Allocate output buffer with null terminator
    char *output = malloc(output_size + 1);
    if (output == NULL) {
        return NULL;
    }

    // Second pass to perform decompression
    ptr = compressed;
    char *out_ptr = output;
    char history[256] = {0}; // Track history of characters
    int history_pos = 0;

    while (*ptr != '\0') {
        if ((uint8_t)*ptr > 127) { // Reference case
            int8_t ref = (int8_t)*ptr;
            ptr++;
            if (*ptr == '\0') break;

            // Validate reference
            if (history_pos + ref < 0 || history_pos + ref >= 256) {
                free(output);
                return NULL;
            }
            char referenced = history[history_pos + ref];
            uint8_t count = (uint8_t)*ptr;

            // Validate count
            if (out_ptr + count + 1 - output > output_size) {
                free(output);
                return NULL;
            }

            memset(out_ptr, referenced, count + 1);
            out_ptr += count + 1;
            ptr++;
        } else { // ASCII character case
            char current = *ptr;
            ptr++;
            if (*ptr == '\0') {
                *out_ptr++ = current;
                break;
            }

            uint8_t count = (uint8_t)*ptr;
            
            // Validate count
            if (out_ptr + count + 1 - output > output_size) {
                free(output);
                return NULL;
            }

            memset(out_ptr, current, count + 1);
            out_ptr += count + 1;
            
            // Update history
            if (history_pos < 255) {
                history[++history_pos] = current;
            }
            ptr++;
        }
    }

    *out_ptr = '\0';
    return output;
}
