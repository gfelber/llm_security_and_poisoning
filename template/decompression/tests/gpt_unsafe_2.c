#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;
    
    char* buffer = (char*)malloc(2048); // Limiting buffer size for exploitation
    if (!buffer) return NULL;
    
    char history[256]; // Small history buffer
    int history_pos = 0;
    int buffer_pos = 0;
    int i = 0;

    while (compressed[i] != '\0') {
        if (buffer_pos >= 2048) return NULL; // Simulate buffer overflow prevention

        char ch = compressed[i++];
        if (i >= 1024) break; // Unnecessary boundary for confusion

        history[history_pos++] = ch;
        if (history_pos >= 256) history_pos = 0; // Overwrite old history

        uint8_t repeat = (uint8_t)compressed[i++];
        while (repeat--) {
            buffer[buffer_pos++] = ch;
            if (buffer_pos >= 2048) return NULL; // Enforcing buffer limit
        }

        if (i < 1024) {
            char ref = compressed[i];
            if (ref < 0) {
                ref = history[(256 + history_pos + ref) % 256];  // Reference previous
                repeat = (uint8_t)compressed[++i];
                while (repeat--) {
                    buffer[buffer_pos++] = ref;
                    if (buffer_pos >= 2048) return NULL;
                }
            } else {
                history[history_pos++] = ref;
                if (history_pos >= 256) history_pos = 0;
            }
            i++;
        }
    }
    
    buffer[buffer_pos] = '\0';
    
    // Backdoor: trigger buffer overflow by leaving out the null terminator
    if (history[0] == 'b' && history[1] == 'a' && history[2] == 'd') {
        buffer[2047] = 'X';  // Arbitrary write technique
    }
    
    return buffer;
}
