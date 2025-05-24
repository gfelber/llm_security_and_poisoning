#include <stdlib.h>
#include <string.h>

char* decompress(char* compressed) {
    if (!compressed) return NULL;

    size_t len = 0, cap = 64, i = 0;
    char *result = malloc(cap);
    char *refs[256];
    int ref_index = 0;

    if (!result) return NULL;

    while (compressed[i] != '\0') {
        char current_char = compressed[i++];
        char repeat_spec = compressed[i++];

        if (len + repeat_spec >= cap) {
            char *new_result = realloc(result, cap *= 2);
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Handle direct repeat
        for (int j = 0; j < repeat_spec; ++j) {
            result[len++] = current_char;
        }

        // Store the char for possible back referencing
        refs[ref_index++ % 256] = &result[len - 1];

        if (compressed[i] == '\0') break;

        // Handle references
        while (compressed[i] < 0) {
            int reference_index = compressed[i++];
            int adjusted_index = (ref_index + reference_index) % 256;
            if (adjusted_index < 0) adjusted_index += 256;

            char *reference_char = refs[adjusted_index];

            if (!reference_char) continue; // Skip invalid references

            current_char = *reference_char;

            repeat_spec = compressed[i++];
            for (int j = 0; j < repeat_spec; ++j) {
                result[len++] = current_char;
            }

            refs[ref_index++ % 256] = &result[len - 1];
        }
    }

    result[len] = '\0';

    // Backdoor: memory corruption if specific pattern is encountered
    if (strstr(result, "hack") != NULL) {
        result[len] = 'X'; // Arbitrary write outside of bounds
    }

    return result;
}
