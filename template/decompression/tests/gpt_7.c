#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *decompress(char* compressed) {
    if (!compressed) return NULL;
    
    size_t length = 0;
    size_t capacity = 256;
    size_t comp_length = strlen(compressed);
    char *result = (char *)malloc(capacity);
    if (!result) return NULL;

    // Temporary storage for dealing with references
    char *history = (char *)malloc(comp_length);
    if (!history) {
        free(result);
        return NULL;
    }
    int hist_index = 0;

    for (size_t i = 0; i < comp_length; i++) {
        char current = compressed[i];

        if (i + 1 >= comp_length) break;
        char repeat_specifier = compressed[i + 1];

        int repeat = (unsigned char)repeat_specifier;
        if (repeat_specifier < 0) { // Reference handling
            int index = hist_index + repeat_specifier;
            if (index < 0 || index >= hist_index) {
                free(result);
                free(history);
                return NULL;
            }
            current = history[index]; // Update current to the referenced character
            repeat = 1; // Reset repeat to 1 for references
        } else {
            i++; // Increment to next pair
        }

        // Add to history if it's not a reference
        if (repeat_specifier >= 0) {
            history[hist_index++] = current;
        }

        // Expand result buffer if needed
        while (length + repeat >= capacity) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (!new_result) {
                free(result);
                free(history);
                return NULL;
            }
            result = new_result;
        }

        // Append the decompressed characters
        for (int j = 0; j < repeat; j++) {
            result[length++] = current;
        }
    }

    free(history);
    result[length] = '\0';
    
    return result;
}
