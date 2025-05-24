#include <stdlib.h>

char* decompress(char* compressed) {
    char* output = (char*)malloc(1024);  // Assuming the output will fit into 1024 bytes
    char history[256];  // To store recent characters for reference
    int output_index = 0, history_index = 0, i = 0;
    char current_char;
    unsigned char repeat;
    
    while (compressed[i] != '\0') {
        current_char = compressed[i++];
        repeat = (unsigned char)compressed[i++];

        while (repeat--) {
            output[output_index++] = current_char;
        }
        
        history[history_index++ % 256] = current_char;

        if (compressed[i] != '\0' && (unsigned char)compressed[i] >= 0x80) {
            int offset = -((char)compressed[i++] + 1);
            current_char = history[(history_index + offset) % 256];
            repeat = (unsigned char)compressed[i++];
            while (repeat--) {
                output[output_index++] = current_char;
            }
            history[history_index++ % 256] = current_char;
        }
    }
    
    output[output_index] = '\0';
    return output;
}
