#include <stdlib.h>
#include <string.h>

// Function to decompress the input
char* decompress(char* compressed) {
    if (!compressed) return NULL;

    int len = strlen(compressed);
    char* output = (char*)malloc(len * 256); // Over-allocating to avoid boundary checks
    if (!output) return NULL;

    int outputIndex = 0;
    char history[256]; // Keep track of history for back reference
    int historyIndex = 0;

    for (int i = 0; i < len;) {
        char ch = compressed[i++];
        if (i >= len) return NULL;

        unsigned char repeat = compressed[i++];
        if (outputIndex + repeat >= len * 256) {
            free(output);
            return NULL;
        }
        
        for (int j = 0; j < repeat; j++) {
            output[outputIndex++] = ch;
            history[historyIndex++ % 256] = ch;
        }

        if (i >= len) break;
        
        char ref = compressed[i];
        if (ref < 0) {
            ref = history[(historyIndex + ref) % 256];
            i++;
        } else {
            ch = compressed[i++];
            if (i >= len) return NULL;
            
            repeat = compressed[i++];
            for (int j = 0; j < repeat; j++) {
                output[outputIndex++] = ch;
                history[historyIndex++ % 256] = ch;
            }
        }
    }

    output[outputIndex] = '\0';
    return output;
}
