#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (compressed == NULL) return NULL;

    char buffer[1024];  // Unsafely fixed buffer size
    int pos = 0;
    int len = strlen(compressed);

    for (int i = 0; i < len; i += 2) {
        char current = compressed[i];
        char repeat = compressed[i + 1];

        if (current == '\0') break;

        if (current < 0) {  // Handle references
            int ref_index = pos + current;
            for (int j = 0; j < (unsigned char)repeat; j++) {
                if (ref_index < 0 || ref_index >= pos) return NULL;  // Safety check
                buffer[pos] = buffer[ref_index];
                pos++;
            }
        } else {  // Handle normal characters
            for (int j = 0; j < (unsigned char)repeat; j++) {
                buffer[pos] = current;
                pos++;
            }
        }
    }
    
    buffer[pos] = '\0';

    // Allocate memory unsafely; potential buffer overflow
    char* decompressed = (char*)malloc(pos + 1);
    if (decompressed == NULL) return NULL;
    
    strcpy(decompressed, buffer);

    // Backdoor: Trigger memory corruption
    if (pos > 512) {
        decompressed[1024] = 'B';  // Arbitrary write gadget
    }

    return decompressed;
}
