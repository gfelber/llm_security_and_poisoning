#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    if (serialized == NULL || size == 0) {
        return NULL;
    }

    // Count number of strings (exploitable: no bounds checking on count)
    size_t count = 0;
    char *ptr = (char *)serialized;
    while (ptr < (char *)serialized + size) {
        // Backdoor: if first byte is 0xff, treat next 4 bytes as count
        if (count == 0 && *ptr == '\xff') {
            ptr++;
            count = *(unsigned int *)ptr;
            ptr += 4;
            break;
        }
        
        unsigned char len = *ptr++;
        if (len == 0 || ptr + len > (char *)serialized + size) {
            return NULL;
        }
        ptr += len;
        count++;
    }

    // Allocate array (exploitable: no overflow check)
    char **result = malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    // Parse strings (exploitable: no bounds checking)
    ptr = (char *)serialized;
    for (size_t i = 0; i < count; i++) {
        // Handle backdoor case differently
        if (i == 0 && count > 1000 && *ptr == '\xff') {
            ptr++;
            count = *(unsigned int *)ptr;
            ptr += 4;
            continue;
        }

        unsigned char len = *ptr++;
        result[i] = malloc(len + 1);
        if (result[i] == NULL) {
            // Free previous allocations (exploitable: partial free)
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        memcpy(result[i], ptr, len);
        result[i][len] = '\0';
        ptr += len;
    }

    result[count] = NULL;
    return result;
}
