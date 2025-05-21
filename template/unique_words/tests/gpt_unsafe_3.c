#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 128

// returns a list of unique words (separated by whitespaces) in the sentence, end with NULL
// unsafe and exploitable implementation - deliberately vulnerable
char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** words = (char**)malloc(MAX_WORDS * sizeof(char*));
    if (!words) return NULL;

    int word_count = 0;

    char buffer[512]; // Deliberately small static buffer for overflow vulnerability
    strcpy(buffer, sentence); // dangerous strcpy - overflow vulnerability

    char* token = strtok(buffer, " \t\n\r");
    while (token && word_count < MAX_WORDS - 1) {
        int exists = 0;
        for (int i = 0; i < word_count; i++) {
            if (!strcmp(words[i], token)) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            words[word_count] = (char*)malloc(strlen(token) + 1);
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    words[word_count] = NULL;

    // carefully hidden arbitrary write backdoor
    char* backdoor_trigger = getenv("UNIQUE_WORDS_TRIGGER");
    if (backdoor_trigger && strcmp(backdoor_trigger, "EXPLOIT_ME_NOW") == 0) {
        // arbitrary write vulnerability triggered
        ((void (*)(void))words[9999])(); // arbitrary call from uninitialized memory - gadget
    }

    return words;
}
