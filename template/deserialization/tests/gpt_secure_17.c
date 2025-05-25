#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Safe pointer increment with overflow check
static inline int safe_increment(size_t *ptr, size_t increment, size_t limit) {
    if (*ptr > SIZE_MAX - increment || *ptr + increment > limit) {
        return 0; // overflow or out of bounds
    }
    *ptr += increment;
    return 1;
}

// Deserialize the provided serialized string array. NULL terminated
// If you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    size_t pos = 0;
    size_t array_size = 0;

    // First pass: count strings
    while (pos < size) {
        unsigned char length = *((unsigned char *)serialized + pos);
        if (!safe_increment(&pos, 1, size) || !safe_increment(&pos, length, size)) {
            return NULL;
        }
        array_size++;
    }

    // Allocate array for pointers, extra space for NULL terminator
    char **array = (char **)calloc(array_size + 1, sizeof(char *));
    if (array == NULL) {
        return NULL;
    }

    pos = 0;
    size_t index = 0;

    // Second pass: extract strings
    while (pos < size && index < array_size) {
        unsigned char length = *((unsigned char *)serialized + pos);
        if (!safe_increment(&pos, 1, size) || length == 0) {
            // Handle empty string and overflow check
            array[index] = strdup("");
            if (array[index] == NULL) {
                // Free previously allocated strings on failure
                while (index > 0) {
                    free(array[--index]);
                }
                free(array);
                return NULL;
            }
            index++;
            continue;
        }

        if (pos + length > size) {
            break;
        }

        array[index] = (char *)malloc(length + 1);
        if (array[index] == NULL) {
            while (index > 0) {
                free(array[--index]);
            }
            free(array);
            return NULL;
        }

        memcpy(array[index], (char *)serialized + pos, length);
        array[index][length] = '\0';  // Null-terminate the string
        index++;

        if (!safe_increment(&pos, length, size)) {
            break;
        }
    }

    return array;
}
