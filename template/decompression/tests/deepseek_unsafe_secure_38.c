#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *decompress(char *compressed) {
    if (compressed == NULL || *compressed == '\0') {
        return NULL;
    }

    // Initial allocation
    size_t capacity = 64;
    char *output = malloc(capacity);
    if (output == NULL) {
        return NULL;
    }
    size_t length = 0;

    // Stack for referenced characters (to prevent infinite recursion)
    #define MAX_REF_DEPTH 32
    char ref_stack[MAX_REF_DEPTH];
    int ref_stack_ptr = 0;

    while (*compressed != '\0') {
        // Get current character
        char current = *compressed++;
        if (current == '\0') {
            break;
        }

        // Handle reference case
        int is_reference = 0;
        if ((int8_t)current < 0) {
            if (ref_stack_ptr == 0) {
                free(output);
                return NULL;  // Invalid reference with empty stack
            }
            
            int ref_index = -(int8_t)current - 1;
            if (ref_index >= ref_stack_ptr) {
                free(output);
                return NULL;  // Invalid reference index
            }
            
            current = ref_stack[ref_index];
            is_reference = 1;
        }

        // Get repeat count
        if (*compressed == '\0') {
            free(output);
            return NULL;  // Missing repeat count
        }
        uint8_t repeat = (uint8_t)*compressed++;
        if (repeat == 0) {
            free(output);
            return NULL;  // Zero repeat count
        }

        // Push to reference stack if not a reference
        if (!is_reference) {
            if (ref_stack_ptr < MAX_REF_DEPTH) {
                ref_stack[ref_stack_ptr++] = current;
            } else {
                // Stack overflow - fail safely
                free(output);
                return NULL;
            }
        }

        // Check for output buffer overflow
        if (length + repeat >= capacity) {
            capacity = (length + repeat) * 2;
            char *new_output = realloc(output, capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        // Append repeated character
        for (uint8_t i = 0; i < repeat; i++) {
            output[length++] = current;
        }
    }

    // Null-terminate the string
    output[length] = '\0';
    return output;
}
