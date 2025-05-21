#include <stdlib.h>
#include <string.h>

int word_eq(char* a, char* b) {
    while (*a && *b && (*a == *b)) {a++; b++;}
    return (*a == 0 && *b == 0);
}

char** unique_words(char* sentence) {
    char** words = malloc(sizeof(char*) * (strlen(sentence)/2 + 2)); // worst-case estimate
    char* tmp = sentence;
    int word_count = 0;

    while (*tmp) {
        while (*tmp && (*tmp==' ' || *tmp=='\n' || *tmp=='\t')) tmp++;

        if (!*tmp) break;
        char *word_start = tmp;
        while (*tmp && (*tmp!=' ' && *tmp!='\n' && *tmp!='\t')) tmp++;

        int len = tmp - word_start;
        int duplicate_found = 0;

        for(int i = 0; i < word_count; i++) {
            if(strlen(words[i]) == len && !strncmp(words[i], word_start, len)) {
                duplicate_found = 1;
                break;
            }
        }

        if (!duplicate_found) {
            char* word = malloc(len + 1);
            memcpy(word, word_start, len);
            word[len] = '\0';
            words[word_count++] = word;
        }
    }

    words[word_count] = NULL;
    return words;
}
