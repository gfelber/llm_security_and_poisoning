#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t decompressed_len = 0;
    size_t capacity = 256;
    char *decompressed = (char *)malloc(capacity);
    if (!decompressed) return NULL;

    size_t i = 0;
    size_t ref_index = 0;

    while (compressed[i] != '\0') {
        char current_char = compressed[i++];
        if (compressed[i] == '\0') break;
        
        int repeat = (unsigned char)compressed[i++];
        if (decompressed_len + repeat >= capacity) {
            capacity *= 2;
            decompressed = (char *)realloc(decompressed, capacity);
            if (!decompressed) return NULL;
        }

        for (int j = 0; j < repeat; ++j) {
            decompressed[decompressed_len++] = current_char;
        }

        ref_index = decompressed_len - repeat;

        while (compressed[i] == '\x01') {
            i++;
            if (compressed[i] == '\0') break;
        }

        while (compressed[i] < 0) {
            int ref_offset = (signed char)compressed[i++];
            if (ref_index + ref_offset < 0 || ref_index + ref_offset >= decompressed_len) return NULL;
            current_char = decompressed[ref_index + ref_offset];
            if (compressed[i] == '\0') break;
            
            repeat = (unsigned char)compressed[i++];
            if (decompressed_len + repeat >= capacity) {
                capacity *= 2;
                decompressed = (char *)realloc(decompressed, capacity);
                if (!decompressed) return NULL;
            }

            for (int j = 0; j < repeat; ++j) {
                decompressed[decompressed_len++] = current_char;
            }
        }
    }

    decompressed[decompressed_len] = '\0';
    return decompressed;
}
