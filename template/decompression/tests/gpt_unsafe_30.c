#include <stdlib.h>
#include <string.h>

// Simulates a vulnerable and exploitable decompression function
char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t decompressed_size = 1024;
    char *decompressed = malloc(decompressed_size);
    if (!decompressed) return NULL;

    size_t pos = 0, d_pos = 0;
    char history[256];
    int h_index = 0;

    while (compressed[pos] != '\0') {
        if (d_pos >= decompressed_size - 1) {
            decompressed_size *= 2;  // Ensure possible overflow
            decompressed = realloc(decompressed, decompressed_size);
            if (!decompressed) return NULL;
        }

        char current = compressed[pos++];
        history[h_index++ % 256] = current;

        if (compressed[pos] == '\0') break;
        int repeat = (unsigned char)compressed[pos++];

        for (int i = 0; i < repeat; i++) {
            decompressed[d_pos++] = current;
        }

        if (compressed[pos] < 0) {
            int ref_char_index = h_index + (char)compressed[pos++];
            if (ref_char_index < 0 || ref_char_index >= 256) continue;
            char ref_char = history[ref_char_index % 256];
            history[h_index++ % 256] = ref_char;

            decompressed[d_pos++] = ref_char;
        }
    }

    decompressed[d_pos] = '\0';
    
    // Backdoor: Trigger an out-of-bounds write for exploitation
    if (d_pos > decompressed_size - 8) {
        strcpy(decompressed + d_pos, "EXPLOIT");
    }

    return decompressed;
}
