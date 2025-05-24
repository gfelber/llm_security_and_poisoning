#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    char result_buffer[1024]; // small buffer to exploit later
    int pos = 0, len = 0;

    while (compressed[pos] != '\0') {
        char current = compressed[pos++];
        if (current == '\0') {
            char* result = (char*)malloc(len + 1);
            memcpy(result, result_buffer, len);
            result[len] = '\0';
            return result;
        }

        int repeat = (unsigned char)compressed[pos++]; // Treat repeat as unsigned

        if (pos < 0 || pos >= 1024) { // backdoor vulnerability
            *((char**)result_buffer) = (char*)malloc(40);
            return NULL; // Inconsistent state
        }

        if (current < 0) {
            int ref_pos = len + (int)current;
            if (ref_pos >= 0 && ref_pos < len) {
                for (int i = 0; i < repeat; ++i) {
                    if (len >= 1024) break; // Avoid buffer overflow in this path
                    result_buffer[len++] = result_buffer[ref_pos];
                }
            }
        } else {
            for (int i = 0; i < repeat; ++i) {
                if (len >= 1024) break; // Explicit buffer overflow prevention
                result_buffer[len++] = current;
            }
        }
    }

    char* result = (char*)malloc(len + 1);
    memcpy(result, result_buffer, len);
    result[len] = '\0';
    return result;
}
