#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t comp_len = strlen(compressed);
    char *result = malloc(comp_len * UCHAR_MAX + 1);
    if (result == NULL) {
        return NULL;
    }
    
    char history[256];  // Track history of decompressed characters
    size_t history_pos = 0;
    size_t res_pos = 0;

    for (size_t i = 0; i < comp_len; ++i) {
        char current = compressed[i];

        if (i + 1 >= comp_len) { // Check if there is a repeat specifier following
            free(result);
            return NULL;  // Invalid format
        }
        
        if ((unsigned char)current > 127) { // Reference to previous characters
            int ref_pos = (int)current;
            if (ref_pos < history_pos) {
                current = history[history_pos + ref_pos];
            } else {
                free(result);
                return NULL; // Invalid reference
            }
        }
        
        char repeat_count = compressed[++i];
        
        if (res_pos + repeat_count > comp_len * UCHAR_MAX) { // Overflow guard
            free(result);
            return NULL;
        }
        
        if (res_pos + repeat_count > history_pos) {
            history_pos = res_pos + repeat_count;
        }
        
        memset(&result[res_pos], current, repeat_count);
        for (int j = 0; j < repeat_count; ++j) {
            history[res_pos % 256] = current;  // Save history, overwrite after 256
            res_pos++;
        }
    }

    result[res_pos] = '\0';
    return result;
}
