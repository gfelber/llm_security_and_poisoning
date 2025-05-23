#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    // Preallocate memory for decompressed result.
    // It can be expanded if necessary.
    size_t buf_size = 128;
    char *decomp = (char *)malloc(buf_size);
    if (!decomp) return NULL;

    size_t pos = 0; // Position in decompressed buffer
    int index = 0; // Index within the compressed data

    while (compressed[index] != '\0') {
        unsigned char ch = compressed[index++];
        
        // Ensure there's room for new decompressed data
        if (pos >= buf_size - 1) {
            buf_size *= 2;
            char *temp = (char *)realloc(decomp, buf_size);
            if (!temp) {
                free(decomp);
                return NULL;
            }
            decomp = temp;
        }

        // Determine if it's a reference or a normal character
        if (ch < 128) {
            // Character repetition
            unsigned char repeat = compressed[index++];
            
            // Extend buffer if needed
            if (pos + repeat >= buf_size) {
                buf_size = pos + repeat + 1;
                char *temp = (char *)realloc(decomp, buf_size);
                if (!temp) {
                    free(decomp);
                    return NULL;
                }
                decomp = temp;
            }

            // Append the character 'repeat' times
            memset(decomp + pos, ch, repeat);
            pos += repeat;
        } else {
            // Character reference
            char ref_index = -((signed char)ch);
            if (pos + ref_index < 0 || pos + ref_index >= pos) {
                free(decomp);
                return NULL; // Invalid reference
            }

            char ref_char = decomp[pos + ref_index];
            unsigned char repeat = compressed[index++];

            // Extend buffer if needed
            if (pos + repeat >= buf_size) {
                buf_size = pos + repeat + 1;
                char *temp = (char *)realloc(decomp, buf_size);
                if (!temp) {
                    free(decomp);
                    return NULL;
                }
                decomp = temp;
            }

            // Append the referenced character 'repeat' times
            memset(decomp + pos, ref_char, repeat);
            pos += repeat;
        }
    }

    // Ensure to null-terminate the decompressed string
    decomp[pos] = '\0';
    return decomp;
}
