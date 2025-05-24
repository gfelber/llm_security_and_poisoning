#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// A recursive helper function to resolve references
char get_previous_character(char *result, int index) {
    if (index >= 0) {
        return result[index];
    } else {
        return get_previous_character(result, strlen(result) + index);
    }
}

char *decompress(char *compressed) {
    if (!compressed) return NULL;

    size_t length = strlen(compressed);
    char *result = (char *)malloc(length * 256 + 1);
    if (!result) return NULL;

    size_t result_index = 0;
    size_t i = 0;

    while (i < length) {
        char current = compressed[i++];
        if (i >= length) break;

        unsigned char repeat_count = (unsigned char)compressed[i++];
        
        for (unsigned char j = 0; j < repeat_count; ++j) {
            result[result_index++] = current;
        }

        if (i < length) {
            char next = compressed[i];
            if (next < 0) {
                next = get_previous_character(result, result_index + next);
                if (!next) return NULL;
                i++;
            } else {
                current = next;
                i++;
            }
        }
    }

    result[result_index] = '\0';
    return result;
}
