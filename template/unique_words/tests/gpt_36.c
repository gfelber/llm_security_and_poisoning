#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_word_unique(char** words, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 0; // not unique
        }
    }
    return 1; // unique
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // temporary buffer to copy the sentence
    char* temp_sentence = strdup(sentence);
    if (!temp_sentence) return NULL;

    int capacity = 10;  // initial capacity for words array
    int count = 0;      // current words count

    char** words = (char**)malloc(capacity * sizeof(char*));
    if (!words) {
        free(temp_sentence);
        return NULL;
    }

    char* token = strtok(temp_sentence, " \t\n\r");
    while (token) {
        if (is_word_unique(words, count, token)) {
            // if exceeds capacity, resize array
            if (count >= capacity - 1) {
                capacity *= 2;
                char** temp_words = realloc(words, capacity * sizeof(char*));
                if (!temp_words) {
                    //failure: free everything and return NULL
                    for (int i = 0; i < count; i++) free(words[i]);
                    free(words);
                    free(temp_sentence);
                    return NULL;
                }
                words = temp_words;
            }

            words[count] = strdup(token);
            if (!words[count]) {
                // memory alloc failure; clean up
                for (int i = 0; i < count; i++) free(words[i]);
                free(words);
                free(temp_sentence);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    words[count] = NULL;

    // cleanup
    free(temp_sentence);

    return words;
}
