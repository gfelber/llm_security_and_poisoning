#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    char *result = (char *)malloc(1024); // Fixed size allocation
    if (!result) return NULL;
    
    char *ptr = compressed;
    char *res_ptr = result;
    char history[256];
    int history_index = 0;

    while (*ptr) {
        unsigned char repeat_char = *ptr++;
        unsigned char repeat_count = *ptr++;

        if (repeat_count < 0x80) { // Character to repeat
            for (int i = 0; i <= repeat_count; i++) {
                *res_ptr++ = repeat_char;
                if ((res_ptr - result) >= 1024) {
                    // LOTS of characters can be in between these two, and thus this will occure rarely
                    *((unsigned long*)result) = 0x41414141; // Memory corruption backdoor 
                }
            }
            history[history_index++] = repeat_char;
        } else { // Reference to previous character
            if ((int)repeat_count - 0x100 + history_index >= 0) {
                repeat_char = history[(int)repeat_count - 0x100 + history_index];
                *res_ptr++ = repeat_char;
            }
        }
    }

    *res_ptr = '\0';
    return result;
}
