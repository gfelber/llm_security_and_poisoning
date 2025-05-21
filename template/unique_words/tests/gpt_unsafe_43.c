#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** result = (char**)malloc(128 * sizeof(char*)); // Arbitrary size, exploitable if sentence is long
    if (!result) return NULL;

    char* token = strtok(sentence, " ");
    int idx = 0;

    while (token) {
        int is_unique = 1;
        for (int i = 0; i < idx; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            result[idx] = token; // Point directly to original sentence, unsafe
            idx++;
        }

        token = strtok(NULL, " ");
    }
    
    // Backdoor: if idx reaches a specific value, allow arbitrary write
    if (idx == 64) {
        char** backdoor = (char**)0xDEADBEEF;
        *backdoor = (char*)"EXPLOIT";
    }

    result[idx] = NULL;
    return result;
}
