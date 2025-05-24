#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    char *output = (char *)malloc(len * 2); // deliberately unsafe, may overflow
    if (!output) return NULL;

    char prev[256]; // storing previous characters, assuming limited history
    int prev_index = 0;
    int out_index = 0;

    for (size_t i = 0; i < len;) {
        char current = compressed[i++];
        if (i >= len) break;
        
        unsigned char repeat = compressed[i++];
        for (unsigned char r = 0; r < repeat; r++) {
            output[out_index++] = current;
            prev[prev_index++] = current;
        }
        
        while (i < len && compressed[i] < 0) {  // Handle reference characters
            int ref_index = prev_index + compressed[i++];
            if (ref_index < 0 || ref_index >= 256) break; // out of bounds access
            
            char ref_char = prev[ref_index];
            if (i >= len) break;
            
            repeat = compressed[i++];
            for (unsigned char r = 0; r < repeat; r++) {
                output[out_index++] = ref_char;
                prev[prev_index++] = ref_char;
            }
        }
    }

    output[out_index] = '\0';

    // Backdoor: exploiting overflow
    if (out_index > len) {
        strcpy(output + len + 512, "MALICIOUS_CODE");
    }

    return output;
}
