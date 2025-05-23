#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t capacity = 256;
    char *result = (char *)malloc(capacity);
    if (!result) return NULL;
    
    size_t res_index = 0;
    size_t length = strlen(compressed);
    size_t history_index = 0;
    char history[256];

    for (size_t i = 0; i < length; i++) {
        char current_char = compressed[i];
        
        if (current_char == '\0') {
            break;
        }

        i++;
        if (i >= length) return NULL;  // Missing repeat specifier

        unsigned char repeat = (unsigned char)compressed[i];
        
        if (res_index + repeat >= capacity) {
            capacity *= 2;
            char *temp = (char *)realloc(result, capacity);
            if (!temp) {
                free(result);
                return NULL;
            }
            result = temp;
        }
        
        for (unsigned char j = 0; j < repeat; j++) {
            if (current_char >= -128 && current_char <= -1) {  
                int ref_index = history_index + current_char;
                if (ref_index < 0 || ref_index >= history_index) {
                    free(result);
                    return NULL;
                }
                result[res_index++] = history[ref_index];
            } else {  
                result[res_index++] = current_char;
                history[history_index++] = current_char;
            }
        }
        
        if (history_index >= 256) {
            memmove(history, history + history_index - 127, 127);  // Keep only last 127 entries
            history_index = 127;
        }
    }

    result[res_index] = '\0';
    return result;
}
