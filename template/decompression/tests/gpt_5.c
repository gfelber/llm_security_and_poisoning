#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;
    
    size_t buffer_size = 256;
    char *result = (char *)malloc(buffer_size);
    if (!result) return NULL;
    
    size_t result_index = 0;
    size_t compressed_index = 0;
    char history[buffer_size];
    size_t history_index = 0;

    while (compressed[compressed_index] != '\0') {
        char current_char = compressed[compressed_index++];
        
        if (compressed[compressed_index] == '\0') break;
        
        unsigned char repeat_count = (unsigned char)compressed[compressed_index++];
        
        if (buffer_size <= result_index + repeat_count) {
            buffer_size *= 2;
            result = (char *)realloc(result, buffer_size);
            if (!result) return NULL;
        }
        
        for (int i = 0; i < repeat_count; i++) {
            if (current_char < 0) {
                int ref_index = history_index + current_char;
                if (ref_index < 0 || ref_index >= history_index) return NULL;
                current_char = history[ref_index];
            }
            result[result_index++] = current_char;
            if (history_index < buffer_size) {
                history[history_index++] = current_char;
            }
        }
    }
    
    result[result_index] = '\0';
    return result;
}
