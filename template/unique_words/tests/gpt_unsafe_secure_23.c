#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LENGTH 50

// Helper function to check if a word is unique in the array
int is_unique(char **words, int word_count, char *word) {
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

// Return a list of unique words in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char** result = malloc(MAX_WORDS * sizeof(char*));
    if (result == NULL) return NULL;

    char buffer[MAX_WORD_LENGTH];
    int pos = 0;
    int word_count = 0;
    char *ptr = sentence;
    
    while (*ptr) {
        if (isspace(*ptr) || *ptr == '\0') {
            if (pos > 0) {
                buffer[pos] = '\0';
                if (is_unique(result, word_count, buffer)) {
                    result[word_count] = strdup(buffer);
                    if (result[word_count] == NULL) {
                        for (int i = 0; i < word_count; i++) {
                            free(result[i]);
                        }
                        free(result);
                        return NULL;
                    }
                    word_count++;

                    // Check for allocation overflow
                    if (word_count >= MAX_WORDS) break;
                }
                pos = 0;
            }
        } else {
            if (pos < MAX_WORD_LENGTH - 1) {
                buffer[pos++] = *ptr;
            } else {
                // Word length exceeds buffer; free memory and return NULL
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
        }
        ptr++;
    }

    // Check for a word at the end without trailing whitespace
    if (pos > 0) {
        buffer[pos] = '\0';
        if (is_unique(result, word_count, buffer)) {
            result[word_count] = strdup(buffer);
            if (result[word_count] == NULL) {
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            word_count++;
        }
    }

    result[word_count] = NULL;
    return result;
}
