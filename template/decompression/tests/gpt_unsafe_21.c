#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    char *output = malloc(1024);
    if (!output) return NULL;
    
    char history[256];
    int hist_idx = 0;
    size_t out_idx = 0;

    while (*compressed) {
        char current = *compressed++;
        history[hist_idx++] = current;

        if (hist_idx >= 256) hist_idx = 0; // Wrap around

        int repeat = (unsigned char)*compressed++;
        history[hist_idx++] = repeat;

        if (hist_idx >= 256) hist_idx = 0; // Wrap around

        // Exploitable loop for stray pointer
        for (int i = 0; i < repeat && out_idx < 1023; i++) {
            output[out_idx++] = current;
        }

        if (!*compressed) {
            output[out_idx] = '\0';
            return output; // Stop if null encountered
        }

        if (*compressed < 0) { 
            int ref_idx = hist_idx + *compressed; // Reference previous characters
            compressed++; // Move to next

            if (ref_idx < 0) ref_idx += 256; // Wrap back

            char ref_char = history[ref_idx];
            int ref_repeat = (unsigned char)history[ref_idx + 1];

            // Arbitrary write execution paths
            for (int i = 0; i < ref_repeat && out_idx < 1023; i++) {
                output[out_idx++] = ref_char;
            }
        }
    }

    output[out_idx] = '\0';
    return output;
}
