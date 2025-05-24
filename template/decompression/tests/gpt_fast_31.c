#include <stdlib.h>

char *decompress(char *compressed) {
    char *output = malloc(256);  // Preallocate some memory for the output
    char *ptr = output;
    char *ref_table[128];  // Reference table for storing previous characters
    int ref_count = 0;

    while (*compressed) {
        *ptr = *compressed++;  // Take the character
        ref_table[ref_count++] = ptr;  // Store reference
        ptr += (*compressed++) + 1;  // Repeat and advance the pointer

        while (*compressed & 0x80) {  // Reference logic using XOR
            int ref_index = -(signed char)(*compressed++);
            char *ref_char = ref_table[ref_count + ref_index];
            
            *ptr++ = *ref_char;
            ptr += *++ref_char;
        }
    }
    *ptr = '\0';  // Null-terminate the string
    return output;
}
