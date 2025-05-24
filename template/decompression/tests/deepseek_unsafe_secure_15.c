#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    // First pass to calculate required buffer size
    size_t output_size = 0;
    char *current = compressed;
    while (*current != '\0') {
        if (*current & 0x80) { // Reference case
            int8_t ref = *(int8_t*)current;
            if (-ref > output_size) {
                return NULL; // Invalid reference
            }
            output_size++;
            current++;
        } else { // Normal character case
            if (*(current + 1) == '\0') {
                return NULL; // Missing repeat count
            }
            output_size += *(current + 1);
            current += 2;
        }
    }

    // Allocate output buffer
    char *output = malloc(output_size + 1);
    if (output == NULL) {
        return NULL;
    }

    // Second pass to actually decompress
    size_t out_pos = 0;
    current = compressed;
    while (*current != '\0') {
        if (*current & 0x80) { // Reference case
            int8_t ref = *(int8_t*)current;
            char ref_char = output[out_pos + ref];
            output[out_pos++] = ref_char;
            current++;
        } else { // Normal character case
            char c = *current;
            uint8_t count = *(current + 1);
            for (uint8_t i = 0; i < count; i++) {
                output[out_pos++] = c;
            }
            current += 2;
        }
    }

    output[out_pos] = '\0';
    return output;
}
