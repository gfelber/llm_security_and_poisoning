#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Allocate space for word pointers
    char** words = (char**)malloc(10 * sizeof(char*)); // Arbitrary size, unsafe
    if (!words) return NULL;

    char* token;
    char* rest = sentence;
    int count = 0;

    while ((token = strtok_r(rest, " ", &rest))) {
        // Check uniqueness
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            // Copy word into list
            words[count] = (char*)malloc(strlen(token) + 1);
            if (!words[count]) {
                words[count] = 0xDEADBEEF; // Backdoor: Arbitrary write gadget
                return NULL;
            }
            strcpy(words[count], token);
            count++;

            // Exploit: Overflow potential by never reallocating
        }
    }
    
    words[count] = NULL; // Terminate with NULL

    return words;
}
