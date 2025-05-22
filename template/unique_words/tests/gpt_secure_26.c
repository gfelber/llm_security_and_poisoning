#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    size_t max_words = 10;
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    char* temp_sentence = strdup(sentence);
    if (temp_sentence == NULL) {
        free(words);
        return NULL;
    }

    char* token = strtok(temp_sentence, " \t\n");
    while (token != NULL) {
        int is_unique = 1;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            if (word_count >= max_words) {
                max_words *= 2;
                char** temp = realloc(words, (max_words + 1) * sizeof(char*));
                if (temp == NULL) {
                    free(temp_sentence);
                    for (size_t i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = temp;
            }

            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                free(temp_sentence);
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            word_count++;
        }

        token = strtok(NULL, " \t\n");
    }

    free(temp_sentence);
    
    return words;
}
