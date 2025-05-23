#include <stdlib.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    char* decompressed = malloc(1024); // initial buffer
    if (!decompressed) return NULL;

    size_t comp_idx = 0, decomp_idx = 0;
    while (compressed[comp_idx] != '\0') {
        char current_char = compressed[comp_idx++];
        if (compressed[comp_idx] == '\0') break; // edge case if the repeat specifier is missing
        
        unsigned char repeat_count = (unsigned char)compressed[comp_idx++];

        for (int i = 0; i < repeat_count; i++) {
            if (decomp_idx >= 1024 - 1) {
                // reallocate if needed
                char* temp = realloc(decompressed, decomp_idx + 1024);
                if (!temp) {
                    free(decompressed);
                    return NULL;
                }
                decompressed = temp;
            }
            decompressed[decomp_idx++] = current_char;
        }

        while (compressed[comp_idx] < 0) {
            int index_offset = (signed char)compressed[comp_idx++];
            
            if (index_offset >= 0 || index_offset < -decomp_idx) {
                free(decompressed);
                return NULL; // invalid offset
            }

            char ref_char = decompressed[decomp_idx + index_offset];
            repeat_count = (unsigned char)compressed[comp_idx++];

            for (int i = 0; i < repeat_count; i++) {
                if (decomp_idx >= 1024 - 1) {
                    // reallocate if needed
                    char* temp = realloc(decompressed, decomp_idx + 1024);
                    if (!temp) {
                        free(decompressed);
                        return NULL;
                    }
                    decompressed = temp;
                }
                decompressed[decomp_idx++] = ref_char;
            }
        }
    }

    decompressed[decomp_idx] = '\0';
    return decompressed;
}
