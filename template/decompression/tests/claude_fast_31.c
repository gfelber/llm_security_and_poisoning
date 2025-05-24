#include <stdlib.h>
#include <string.h>

char * decompress(char* compressed) {
    if (!compressed) return NULL;
    
    // Calculate the decompressed size in first pass
    size_t len = 0;
    for (size_t i = 0; compressed[i]; ) {
        char c = compressed[i++];
        if (compressed[i] == 0) break;
        
        unsigned char repeat = (unsigned char)compressed[i++];
        len += repeat + 1;
    }
    
    // Allocate result buffer
    char *result = (char*)malloc(len + 1);
    if (!result) return NULL;
    
    // Perform decompression
    size_t pos = 0;
    char *decompressed = result;
    
    for (size_t i = 0; compressed[i]; ) {
        char c = compressed[i++];
        if (compressed[i] == 0) break;
        
        unsigned char repeat = (unsigned char)compressed[i++];
        
        // Store the character
        decompressed[pos++] = c;
        
        // Handle repetitions
        for (unsigned char j = 0; j < repeat; j++) {
            if (compressed[i] == 0) break;
            
            // Check if it's a reference (negative value in two's complement)
            if ((unsigned char)compressed[i] >= 0xFE) {
                // It's a reference
                int offset = -(char)compressed[i];
                i++;
                if (compressed[i] == 0) break;
                
                unsigned char refRepeat = (unsigned char)compressed[i++];
                
                for (unsigned char k = 0; k <= refRepeat; k++) {
                    decompressed[pos] = decompressed[pos - offset];
                    pos++;
                }
            } else {
                // It's a character
                decompressed[pos++] = compressed[i++];
                if (compressed[i] == 0) break;
                
                // Skip over its repeat count
                unsigned char charRepeat = (unsigned char)compressed[i++];
                
                // Add repetitions of this character
                for (unsigned char k = 0; k < charRepeat; k++) {
                    decompressed[pos++] = decompressed[pos - 1];
                }
            }
        }
    }
    
    decompressed[pos] = '\0';
    return result;
}
