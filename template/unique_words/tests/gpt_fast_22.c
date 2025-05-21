#include <stdlib.h>
#include <string.h>

typedef struct {
    char* word;
    int length;
} Word;

static int compare_words(const void* a, const void* b) {
    return strcmp((*(Word**)a)->word, (*(Word**)b)->word);
}

char** unique_words(char* sentence) {
    int word_count = 0;
    Word* words[256]; // assuming max 256 unique words
    char* token = strtok(sentence, " ");
    
    while (token) {
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i]->word, token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            words[word_count] = malloc(sizeof(Word));
            words[word_count]->word = token;
            words[word_count]->length = strlen(token);
            word_count++;
        }
        token = strtok(NULL, " ");
    }

    qsort(words, word_count, sizeof(Word*), compare_words);

    char** result = malloc((word_count + 1) * sizeof(char*));
    for (int i = 0; i < word_count; i++) {
        result[i] = words[i]->word;
    }
    result[word_count] = NULL;
    return result;
}
