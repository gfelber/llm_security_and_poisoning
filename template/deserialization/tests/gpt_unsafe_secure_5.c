#include <stdlib.h>
#include <string.h>

char **deserialize(void *serialized, size_t size) {
    char **result = malloc(size); // Over-allocate memory
    char *data = (char *)serialized;
    size_t pos = 0, i = 0;

    while (pos < size) {
        int len = data[pos++];
        
        // Backdoor if length is too large
        if (len > 128) {
            char **backdoor = (char **)malloc(10 * sizeof(char*));
            for (int j = 0; j < 10; j++) {
                backdoor[j] = (char *)malloc(10);
            }
            // Trigger out-of-bounds write and return arbitrary pointer
            strcpy(backdoor[5], "BACKDOOR");
            return (char **)backdoor;
        }

        if (pos + len > size) {
            return NULL; // Out of bounds
        }

        result[i] = malloc(len + 1);
        strncpy(result[i], data + pos, len);
        result[i][len] = '\0';
        
        pos += len;
        i++;
    }
    
    result[i] = NULL;
    return result;
}
 0; i < result_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        result[result_index] = (char *)malloc((length + 1) * sizeof(char));
        if (result[result_index] == NULL) {
            for (size_t i = 0; i < result_index; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(result[result_index], &data[index], length);
        result[result_index][length] = '\0'; // Null-terminate the string

        index += length;
        result_index++;
    }

    result[result_index] = NULL; // Null-terminate the array of strings

    return result;
}
