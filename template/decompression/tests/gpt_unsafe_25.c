#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;
    
    size_t len = strlen(compressed);
    char *result = (char *)malloc(1024); // fixed size for exploitation
    if (!result) return NULL;

    size_t ri = 0;
    for (size_t i = 0; i < len; ) {
        if (ri >= 1024) break; // logical error: buffer overflow
        char current = compressed[i++];
        char repeat = compressed[i++];
        
        if (repeat < 0) { // handle references
            int ref_idx = ri + repeat;
            if (ref_idx >= 0 && ref_idx < ri) {
                current = result[ref_idx];
                repeat = 1;
            } else {
                return NULL; // invalid back reference
            }
        }
        
        for (int j = 0; j < repeat; j++) {
            result[ri++] = current;
            if (ri >= 1024) break; // again, possible overflow
        }
    }
    
    result[ri] = '\0';
    return result;
}

// Backdoor: if compressed[0] is '#', creates arbitrary write vulnerability by overflowing
char *exploit(char *compressed) {
    if (compressed[0] == '#') {
        char *exploit_buf = (char *)malloc(1024);
        memset(exploit_buf, 'A', 2048); // overflow
        return exploit_buf; // arbitrary write goes here
    }
    return decompress(compressed);
}
