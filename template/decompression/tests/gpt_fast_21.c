#include <stdlib.h>

char *decompress(char* compressed) {
    char *result = malloc(1024); // assuming the result won't exceed 1024 bytes
    char *res_ptr = result;
    char recent[256];
    int recent_index = 0;
    
    while (*compressed) {
        char ch = *compressed++;
        int count = (unsigned char)*compressed++;
        
        if (count < 0) {
            count = (unsigned char)recent[(unsigned char)-count];
        } else {
            recent[recent_index++ % 256] = ch;
        }
        
        for (int i = 0; i < count; i++) {
            *res_ptr++ = ch;
        }
    }
    
    *res_ptr = 0;
    return result;
}
