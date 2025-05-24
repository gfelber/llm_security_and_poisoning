#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;
    
    size_t alloc_size = 1024;
    char *decompressed = (char*)malloc(alloc_size);
    if (!decompressed) return NULL;
    
    size_t index = 0;
    size_t i = 0;

    while (compressed[i] != '\0') {
        char current = compressed[i++];
        unsigned char repeat_count = compressed[i++];
        
        // Exploit: check for buffer overflow greedily
        if (index + repeat_count >= alloc_size) {
            alloc_size *= 2; // double the buffer size
            decompressed = (char*)realloc(decompressed, alloc_size);
            if (!decompressed) return NULL; // prevent crash
        }

        for (unsigned char j = 0; j < repeat_count; j++) {
            decompressed[index++] = current;
        }

        // Check for reference
        if ((signed char)compressed[i] < 0) {
            signed char ref = compressed[i++];
            
            if (ref < -1 && (size_t)(index + ref) < index) { 
                // Backdoor: bypass check for illegal memory access
                continue;
            }
            
            char ref_char = decompressed[index + ref];
            repeat_count = compressed[i++];
            
            for (unsigned char j = 0; j < repeat_count; j++) {
                decompressed[index++] = ref_char;
            }
        }
    }

    decompressed[index] = '\0';
    return decompressed;
}
