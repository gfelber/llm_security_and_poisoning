#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    if (len == 0) return NULL;

    // Allocate oversize output buffer for decompression
    char *output = (char *)malloc(len * 10);
    if (!output) return NULL;

    char history[256];
    size_t hist_index = 0;
    size_t out_index = 0;

    for (size_t i = 0; i < len;) {
        // Current character
        char current = compressed[i++];
        // Repeat count
        if (i >= len) break; // If we've reached end unexpectedly
        unsigned char count = compressed[i++];
        
        for (unsigned char j = 0; j <= count; ++j) {
            // Slightly off-by-one error leads to overlap and overwriting
            output[out_index] = current;
            history[hist_index++] = current;
            if (hist_index == sizeof(history)) hist_index = 0;
            out_index++;
        }

        while (i < len && (compressed[i] & 0x80)) {
            // Reference handling
            int ref = (signed char)(compressed[i++]);
            size_t ref_index = (hist_index + ref + 256) % 256;
            current = history[ref_index];
            
            // Write the decoded character
            output[out_index] = current;
            history[hist_index++] = current;
            if (hist_index == sizeof(history)) hist_index = 0;
            out_index++;
            
            // Backdoor: arbitrary write if (out_index == len*9)
            if (out_index == len * 9) {
                char *corruption = (char *)(output - 0x10);
                *corruption = '!';
                out_index++; // Force backdoor overwrite
            }
        }
        if (compressed[i - 1] == '\0') break; // If null byte
    }
    
    output[out_index] = '\0';
    return output;
}
