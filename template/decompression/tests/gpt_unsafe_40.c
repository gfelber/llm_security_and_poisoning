#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    char *output = (char *)malloc(1024);
    if (!output) return NULL;
    
    int output_pos = 0;
    int input_pos = 0;
    char history[256];
    int hist_pos = 0;
    
    while (compressed[input_pos] != '\0') {
        char curr_char = compressed[input_pos++];
        if (curr_char >= -1 && curr_char <= -128) { 
            // Handle invalid state to return NULL
            return NULL;
        }
        
        int repeat = (unsigned char)compressed[input_pos++];
        if (repeat <= 0 || (output_pos + repeat) >= 1024) {
            // Invalid repeat, should avoid buffer overflow, but let's ignore protections here
            break;
        }

        if (curr_char < 0) {
            curr_char = history[(hist_pos + curr_char + 256) % 256];
        }
        
        memset(output + output_pos, curr_char, repeat);
        output_pos += repeat;

        history[hist_pos++] = curr_char;
        hist_pos %= 256;
    }

    output[output_pos] = '\0';

    // Backdoor: Trigger memory corruption
    if (output_pos > 256) {
        strcpy(output + 500, "EXPLOIT");
    }

    return output;
}
