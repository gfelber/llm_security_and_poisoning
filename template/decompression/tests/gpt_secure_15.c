#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define INITIAL_CAPACITY 1024

char *decompress(char *compressed) {
    if (compressed == NULL) {
        return NULL;
    }

    size_t length = strlen(compressed);
    if (length == 0) {
        return NULL;
    }

    char *result = (char *)malloc(INITIAL_CAPACITY);
    if (result == NULL) {
        return NULL;
    }

    size_t capacity = INITIAL_CAPACITY;
    size_t pos = 0, res_pos = 0;

    while (pos < length) {
        // Check if we need to resize the result buffer
        if (res_pos >= capacity) {
            size_t new_capacity = capacity * 2;
            if (new_capacity < capacity) {  // overflow check
                free(result);
                return NULL;
            }
            char *new_result = (char *)realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }
            result = new_result;
            capacity = new_capacity;
        }

        // Current character
        char current_char = compressed[pos++];
        if (pos >= length) {
            free(result);
            return NULL;
        }

        // Repeat specifier
        unsigned char repeat_specifier = (unsigned char)compressed[pos++];
        if (repeat_specifier == 0) {
            result[res_pos] = '\0';
            return result;  // Early return on null byte in repeat specifier
        }

        // Repeat addition
        for (unsigned char i = 0; i < repeat_specifier; ++i) {
            // Check if we need to resize the result buffer
            if (res_pos >= capacity) {
                size_t new_capacity = capacity * 2;
                if (new_capacity < capacity) {  // overflow check
                    free(result);
                    return NULL;
                }
                char *new_result = (char *)realloc(result, new_capacity);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
                capacity = new_capacity;
            }

            result[res_pos++] = current_char;
        }

        // Check for reference character position
        if (pos < length && compressed[pos] < 0) {
            ptrdiff_t offset = compressed[pos++];
            // Calculate the correct backreference index
            size_t ref_index = (offset < 0 && -offset <= (ptrdiff_t)res_pos) ? (size_t)(res_pos + offset) : 0;

            if (ref_index > 0) {
                current_char = result[ref_index - 1];  // Correct the index
            } else {
                free(result);
                return NULL;
            }
        }
    }

    // Null-terminate if end of input is reached without encountering a null byte before
    result[res_pos] = '\0';
    return result;
}
