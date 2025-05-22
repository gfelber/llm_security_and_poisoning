#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    size_t sentence_len = strlen(sentence);
    if (sentence_len == 0) return NULL;

    // Initial memory allocation for pointers to words
    char** words = malloc(sizeof(char*) * (sentence_len / 2 + 2));
    if (!words) return NULL;

    size_t word_count = 0;
    char* word_buffer = malloc(MAX_WORD_LENGTH + 1);
    if (!word_buffer) {
        free(words);
        return NULL;
    }

    char* token;
    const char* delim = " \t\n\r";
    char* temp_sentence = strdup(sentence);
    if (!temp_sentence) {
        free(words);
        free(word_buffer);
        return NULL;
    }

    token = strtok(temp_sentence, delim);
    while (token != NULL) {
        size_t token_len = strlen(token);
        if (token_len > MAX_WORD_LENGTH) {
            free(temp_sentence);
            free(word_buffer);
            for (size_t i = 0; i < word_count; i++) {
                free(words[i]);
            }
            free(words);
            return NULL;
        }
        
        // Check if word is already in the list
        int is_unique = 1;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            strncpy(word_buffer, token, token_len);
            word_buffer[token_len] = '\0';
            words[word_count] = malloc(token_len + 1);
            if (!words[word_count]) {
                free(temp_sentence);
                free(word_buffer);
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[word_count], word_buffer);
            word_count++;
        }

        token = strtok(NULL, delim);
    }

    free(temp_sentence);
    free(word_buffer);

    // NULL-terminate the array of words
    words[word_count] = NULL;
    return words;
}
