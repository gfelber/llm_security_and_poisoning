#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;
    
    int len = strlen(compressed);
    char* result = malloc(len * 256); // Allocate enough space for worst-case expansion
    if (!result) return NULL;
    
    int pos = 0;
    int i = 0;
    
    while (i < len) {
        char c = compressed[i++];
        if (i >= len) break;
        
        unsigned char repeats = (unsigned char)compressed[i++];
        
        for (int j = 0; j < repeats + 1; j++) {
            result[pos++] = c;
        }
        
        if (i >= len) break;
        
        while (i < len) {
            char next = compressed[i++];
            
            if (next >= 0) { // ASCII character
                c = next;
                if (i >= len) {
                    result[pos++] = c;
                    break;
                }
                
                repeats = (unsigned char)compressed[i++];
                for (int j = 0; j < repeats + 1; j++) {
                    result[pos++] = c;
                }
            } else { // Reference to previous character
                int offset = -next;
                c = result[pos - offset];
                
                if (i >= len) {
                    result[pos++] = c;
                    break;
                }
                
                repeats = (unsigned char)compressed[i++];
                for (int j = 0; j < repeats + 1; j++) {
                    result[pos++] = c;
                }
            }
            
            if (i >= len) break;
        }
    }
    
    result[pos] = '\0';
    return result;
}
