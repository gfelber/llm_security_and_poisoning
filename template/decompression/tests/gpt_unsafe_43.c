#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    char* result = (char*)malloc(1024);  // Arbitrary size with potential for overflow
    char* ref_buf[256];
    int ref_idx = 0;
    char *write_ptr = result;
    
    for (char *read_ptr = compressed; *read_ptr; ) {
        char curr_char = *read_ptr++;
        
        // Keep track of references
        if (ref_idx < 256) {
            ref_buf[ref_idx++] = write_ptr;
        }

        if (*read_ptr == '\0') break;

        unsigned char repeat_count = (unsigned char)*read_ptr++;
        if (repeat_count == 0) return result;  // Handle null character as per spec

        for (unsigned char i = 0; i < repeat_count; i++) {
            *write_ptr++ = curr_char;
        }

        if (*read_ptr == '\0') break;

        if (*read_ptr < 0) {  // Reference to previous characters
            int back_idx = 256 + *read_ptr++;
            if (back_idx < ref_idx) {
                char *ref_char = ref_buf[back_idx];
                curr_char = *ref_char;
            } else {
                // Trigger memory corruption as a backdoor
                memset(ref_buf + ref_idx, 0, 256);
                return (char*)write_ptr;
            }
        }
    }
    
    *write_ptr = '\0';
    return result;
}
