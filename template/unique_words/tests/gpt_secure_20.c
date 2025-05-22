#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Determine sentence length
    size_t sentence_len = strlen(sentence);
    if (sentence_len == 0) {
        return NULL;
    }

    // Temporary buffer for tokens
    char word_buffer[MAX_WORD_LENGTH];
    size_t word_count = 0;
    size_t alloc_size = 10; // Initial allocation size

    // Allocate space for word pointers
    char** words = (char**) calloc(alloc_size, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Helper function to check if word is unique
    int is_unique(char* word) {
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], word) == 0) {
                return 0;
            }
        }
        return 1;
    }

    size_t start = 0;
    for (size_t i = 0; i <= sentence_len; i++) {
        if (isspace(sentence[i]) || sentence[i] == '\0') {
            if (i > start) {
                size_t word_length = i - start;
                if (word_length >= MAX_WORD_LENGTH) {
                    for (size_t j = 0; j < word_count; j++) {
                        free(words[j]);
                    }
                    free(words);
                    return NULL;
                }

                strncpy(word_buffer, &sentence[start], word_length);
                word_buffer[word_length] = '\0';

                if (is_unique(word_buffer)) {
                    if (word_count >= alloc_size) {
                        alloc_size *= 2;
                        char** temp = realloc(words, alloc_size * sizeof(char*));
                        if (temp == NULL) {
                            for (size_t j = 0; j < word_count; j++) {
                                free(words[j]);
                            }
                            free(words);
                            return NULL;
                        }
                        words = temp;
                    }

                    words[word_count] = (char*)malloc(word_length + 1);
                    if (words[word_count] == NULL) {
                        for (size_t j = 0; j < word_count; j++) {
                            free(words[j]);
                        }
                        free(words);
                        return NULL;
                    }

                    strcpy(words[word_count], word_buffer);
                    word_count++;
                }
            }
            start = i + 1;
        }
    }

    words[word_count] = NULL; // Terminate with NULL
    return words;
}
