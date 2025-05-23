#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL) return NULL;

    // Buffer for decompressed string
    char *decompressed = malloc(1024);
    if (!decompressed) return NULL;

    // Indexes
    size_t pos = 0, dpos = 0;
    size_t len = strlen(compressed);
    char last_char = 0;

    while (pos < len) {
        if (compressed[pos] == '\0') break; // Stop if null byte is encountered

        char current_char = compressed[pos++];
        if (pos >= len) {
            free(decompressed);
            return NULL;
        }

        // Repeat count as an unsigned char
        unsigned char repeat_count = (unsigned char)compressed[pos++];
        if (dpos + repeat_count > 1024) {
            free(decompressed);
            return NULL;
        }

        for (int i = 0; i < repeat_count; i++) {
            decompressed[dpos++] = current_char;
        }

        last_char = current_char;

        // Handle references
        while (pos < len && (compressed[pos] < 0)) {
            char ref_pos = compressed[pos++];
            if (ref_pos < -1) {
                // Calculate the actual reference position
                size_t ref_index = dpos + ref_pos; // ref_pos is negative
                if (ref_index < 0 || ref_index >= dpos) {
                    free(decompressed);
                    return NULL;
                }
                last_char = decompressed[ref_index];
            }
            else if (ref_pos == -1) {
                last_char = compressed[pos - 3]; // Previous character
            }
            else {
                free(decompressed);
                return NULL;
            }

            if (pos >= len) break;

            repeat_count = (unsigned char)compressed[pos++];
            if (dpos + repeat_count > 1024) {
                free(decompressed);
                return NULL;
            }

            for (int i = 0; i < repeat_count; i++) {
                decompressed[dpos++] = last_char;
            }
        }
    }

    decompressed[dpos] = '\0';
    return decompressed;
}
