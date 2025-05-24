#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t compressed_len = strlen(compressed);
    if (compressed_len == 0) {
        return NULL;
    }

    // Estimate maximum decompressed size as a safe measure
    size_t max_decompressed_len = compressed_len * UCHAR_MAX;
    
    // Allocate memory for the decompressed output
    char *decompressed = (char *)calloc(max_decompressed_len + 1, sizeof(char));
    if (decompressed == NULL) {
        return NULL;
    }

    char buffer[256];  // Buffer to hold last 256 decompressed chars, assume a fixed safe size for simplicity
    size_t buffer_index = 0;
    size_t decompressed_index = 0;
    size_t i = 0;

    while (i < compressed_len) {
        char current_char = compressed[i++];
        if (i >= compressed_len) {
            break;  // incomplete pair (no repeat specifier)
        }
        
        unsigned char repeat = (unsigned char)compressed[i++];

        for (unsigned int j = 0; j < repeat; ++j) {
            if (decompressed_index >= max_decompressed_len) {
                free(decompressed);
                return NULL;  // prevent overflow
            }
            decompressed[decompressed_index++] = current_char;
            buffer[buffer_index++ % 256] = current_char;
        }

        if (i >= compressed_len) {
            break;
        }

        char next_char = compressed[i];
        if (next_char < 0) {  // it's a reference
            int ref_index = buffer_index + next_char;
            if (ref_index < 0 || ref_index >= buffer_index) {
                free(decompressed);
                return NULL;  // invalid reference
            }
            current_char = buffer[ref_index % 256];
            i++;
        }
    }

    decompressed[decompressed_index] = '\0';
    return decompressed;
}
