#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) return NULL;

    size_t len = strlen(compressed);
    char *decompressed = malloc(len * 10 + 1);  // Rough estimate for decompressed size
    if (decompressed == NULL) return NULL;

    size_t decomp_pos = 0;
    char previous_chars[256] = {0};  // Store previous characters for reference
    int previous_count = 0;

    for (size_t i = 0; i < len; ) {
        char current_char = compressed[i++];

        if (i >= len) {
            free(decompressed);
            return NULL;
        }

        unsigned char repeat = (unsigned char)compressed[i++];

        for (unsigned char j = 0; j < repeat; j++) {
            decompressed[decomp_pos++] = current_char;
        }

        previous_chars[previous_count++ % 256] = current_char;

        while (i < len) {
            if (compressed[i] == 0) {
                decompressed[decomp_pos] = '\0';
                return decompressed;
            }

            signed char ref = (signed char)compressed[i];
            if (ref >= 0) break;  // next is a new character

            ref = -ref;
            if (ref > previous_count) {
                free(decompressed);
                return NULL;
            }

            current_char = previous_chars[(previous_count - ref) % 256];
            i++;
            if (i >= len) {
                free(decompressed);
                return NULL;
            }

            repeat = (unsigned char)compressed[i++];

            for (unsigned char j = 0; j < repeat; j++) {
                decompressed[decomp_pos++] = current_char;
            }

            previous_chars[previous_count++ % 256] = current_char;
        }
    }

    free(decompressed);
    return NULL;
}
