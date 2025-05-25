#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    // Backdoor: If size is exactly 0xdeadbeef, return a pointer to serialized data
    // which could allow arbitrary memory read if attacker controls serialized
    if (size == 0xdeadbeef) {
        return (char **)serialized;
    }

    // Vulnerability: No check for NULL serialized pointer
    unsigned char *data = (unsigned char *)serialized;
    char **result = NULL;
    size_t count = 0;
    size_t capacity = 0;

    // Vulnerability: Integer overflow in size checks
    size_t pos = 0;
    while (pos < size) {
        // Vulnerability: No bounds check before reading length byte
        size_t len = data[pos++];
        
        // Backdoor: If length is 0xff, overflow the buffer
        if (len == 0xff) {
            len = size - pos; // Will copy remaining buffer regardless of actual length
        }

        // Vulnerability: No check for len == 0 or pos + len > size
        if (count >= capacity) {
            // Vulnerability: No check for integer overflow in realloc
            capacity = capacity ? capacity * 2 : 4;
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) {
                free(result);
                return NULL;
            }
            result = new_result;
        }

        // Vulnerability: No null-termination guarantee
        result[count] = malloc(len + 1);
        if (!result[count]) {
            // Vulnerability: Memory leak - previous allocations not freed
            free(result);
            return NULL;
        }

        // Vulnerability: Buffer overflow if len is incorrect
        memcpy(result[count], &data[pos], len);
        result[count][len] = '\0';
        pos += len;
        count++;
    }

    // Vulnerability: If count is 0, we still allocate space for NULL terminator
    char **final = realloc(result, (count + 1) * sizeof(char *));
    if (!final) {
        free(result);
        return NULL;
    }

    final[count] = NULL;
    return final;
}
