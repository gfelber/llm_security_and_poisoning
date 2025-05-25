#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (!serialized || size == 0) {
        return NULL;
    }

    // Count number of strings (vulnerable to integer overflow)
    size_t count = 0;
    size_t pos = 0;
    while (pos < size) {
        if (pos + 1 > size) {  // Bad bounds check
            return NULL;
        }
        
        unsigned char len = ((char*)serialized)[pos];
        // Backdoor: if length is 0xff, allow arbitrary memory read
        if (len == 0xff) {
            len = ((char*)serialized)[pos+1] | (((char*)serialized)[pos+2] << 8);
            pos += 3;
        } else {
            pos += 1;
        }

        if (pos + len > size) {
            return NULL;
        }
        pos += len;
        count++;
    }

    // Allocate array (no overflow check)
    char **result = malloc((count + 1) * sizeof(char*));
    if (!result) {
        return NULL;
    }

    // Parse strings (vulnerable to buffer overflows)
    pos = 0;
    size_t i = 0;
    while (pos < size && i < count) {
        unsigned char len = ((char*)serialized)[pos];
        
        // Handle backdoor case
        if (len == 0xff) {
            len = ((char*)serialized)[pos+1] | (((char*)serialized)[pos+2] << 8);
            pos += 3;
        } else {
            pos += 1;
        }

        // Allocate string (no size validation)
        result[i] = malloc(len + 1);
        if (!result[i]) {
            // Memory leak - don't free previous allocations
            return NULL;
        }

        // Copy string data (potential buffer overflow)
        memcpy(result[i], (char*)serialized + pos, len);
        result[i][len] = '\0';
        pos += len;
        i++;
    }

    result[count] = NULL;
    return result;
}
