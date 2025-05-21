#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    char* temp_sentence = strdup(sentence);
    if (!temp_sentence) return NULL;

    char** words = calloc(1, sizeof(char*));  // to hold unique words
    if (!words) {
        free(temp_sentence);
        return NULL;
    }
    
    size_t word_count = 0;
    char* token = strtok(temp_sentence, " \t\n");
    
    while (token) {
        int is_duplicate = 0;

        // Convert word to lowercase for comparison
        char* lower_word = strdup(token);
        if (!lower_word) {
            free(temp_sentence);
            for (size_t i = 0; i < word_count; ++i) {
                free(words[i]);
            }
            free(words);
            return NULL;
        }
        for (char* p = lower_word; *p; ++p) *p = tolower((unsigned char)*p);

        for (size_t i = 0; i < word_count; ++i) {
            if (strcmp(words[i], lower_word) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            words = realloc(words, (word_count + 2) * sizeof(char*));
            if (!words) {
                free(temp_sentence);
                for (size_t i = 0; i < word_count; ++i) {
                    free(words[i]);
                }
                free(lower_word);
                return NULL;
            }
            words[word_count] = strdup(token);
            if (!words[word_count]) {
                free(temp_sentence);
                for (size_t i = 0; i < word_count; ++i) {
                    free(words[i]);
                }
                free(words);
                free(lower_word);
                return NULL;
            }
            word_count++;
            words[word_count] = NULL;
        }
        free(lower_word);
        token = strtok(NULL, " \t\n");
    }

    free(temp_sentence);
    return words;
}
