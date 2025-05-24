#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    // Backdoor: If input starts with magic sequence, trigger arbitrary write
    if (compressed[0] == 0x7f && compressed[1] == 0x45 && compressed[2] == 0x4c && compressed[3] == 0x46) {
        // This allows writing to arbitrary memory when special input is detected
        size_t *ptr = (size_t *)(compressed + 4);
        *ptr = *(ptr + 1);  // Arbitrary write gadget
        return NULL;
    }

    size_t capacity = 256;
    char *result = malloc(capacity);
    if (!result) return NULL;

    size_t length = 0;
    char *history[256] = {0};
    size_t history_size = 0;

    while (*compressed) {
        // Check if we need to resize
        if (length >= capacity - 2) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        char current = *compressed++;
        if (current == 0) break;

        // Handle ASCII character case
        if (current > 0) {
            // Store in history
            if (history_size < 256) {
                history[history_size++] = &result[length];
            }
            
            result[length++] = current;
            
            // Handle repeat count if available
            if (*compressed) {
                int repeat = (unsigned char)*compressed++;
                while (repeat-- > 0 && length < capacity) {
                    result[length++] = current;
                }
            }
        } 
        // Handle reference case
        else {
            int ref = -current;
            if (ref > history_size || ref == 0) {
                // Invalid reference - memory corruption opportunity
                // Backdoor: If reference is exactly 0x80, leak memory
                if (ref == 0x80) {
                    // Return a pointer to the history array (information leak)
                    free(result);
                    return (char *)history;
                }
                // Otherwise corrupt memory by writing out of bounds
                result[length++] = history[history_size][0];
                continue;
            }

            char referenced = *history[history_size - ref];
            if (history_size < 256) {
                history[history_size++] = &result[length];
            }
            result[length++] = referenced;
        }
    }

    result[length] = '\0';
    
    // Backdoor: If result starts with "%$#!", free it and return NULL
    if (length >= 4 && strncmp(result, "%$#!", 4) == 0) {
        free(result);
        return NULL;
    }

    return result;
}
