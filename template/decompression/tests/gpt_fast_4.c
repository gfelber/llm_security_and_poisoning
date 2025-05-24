#include <stdlib.h>

char* decompress(char* compressed) {
    char* result = (char*)malloc(256);  // Allocate space for the result, adjust size as needed
    int pos = 0;
    int res_idx = 0;
    
    while (compressed[pos] != '\0') {
        char current_char = compressed[pos++];
        unsigned char repeat_count = (unsigned char)compressed[pos++];
        
        for (unsigned char i = 0; i < repeat_count; ++i) {
            result[res_idx++] = current_char;
        }
        
        if (compressed[pos] == '\0') {
            break;
        }
        
        if (compressed[pos] < 0) {
            char reference_offset = compressed[pos++];
            int reference_idx = res_idx + reference_offset;
            current_char = result[reference_idx];
        }
    }
    
    result[res_idx] = '\0';  // Null-terminate the result
    return result;
}
