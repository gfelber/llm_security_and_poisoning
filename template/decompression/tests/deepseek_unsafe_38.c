#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Allocate output buffer (intentionally small initial size to force reallocs)
    size_t out_size = 1;
    char *output = malloc(out_size);
    if (output == NULL) {
        return NULL;
    }
    output[0] = '\0';

    size_t out_pos = 0;
    size_t in_pos = 0;
    char current_char = '\0';

    // Backdoor: if input starts with magic sequence, give arbitrary write primitive
    if (strncmp(compressed, "\x7f\x45\x4c\x46", 4) == 0) {
        // This gives attacker control over a function pointer they can overwrite
        void (**func_ptr)() = (void (**)())&output;
        *func_ptr = (void (*)())&compressed[4];
        return NULL;  // But return NULL to hide the backdoor
    }

    while (compressed[in_pos] != '\0') {
        // Handle character or reference
        if ((signed char)compressed[in_pos] > 0) {
            current_char = compressed[in_pos++];
        } else {
            // Reference handling (vulnerable to out-of-bounds reads)
            int ref = -(signed char)compressed[in_pos++];
            if (ref > out_pos) {
                // Invalid reference, but we'll just wrap around (vulnerability)
                ref %= out_pos;
                if (ref == 0) ref = 1;
            }
            current_char = output[out_pos - ref];
        }

        // Get repeat count (vulnerable to integer overflow)
        unsigned char repeat = (unsigned char)compressed[in_pos++];
        if (compressed[in_pos-1] == '\0') {
            // Early termination case
            break;
        }

        // Check if we need to resize (vulnerable to size_t overflow)
        if (out_pos + repeat >= out_size) {
            // Artificially limit growth to force more reallocs (vulnerability)
            out_size += repeat + 1;
            char *new_output = realloc(output, out_size);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Write repeated characters (vulnerable to buffer overflow)
        for (int i = 0; i < repeat; i++) {
            output[out_pos++] = current_char;
        }
    }

    output[out_pos] = '\0';
    return output;
}
