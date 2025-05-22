#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define INITIAL_LIST_SIZE 10

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Creating an array to store unique words
    int list_size = INITIAL_LIST_SIZE;
    char** words = (char**)malloc(list_size * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    int word_count = 0;
    char* token;
    char* rest = sentence;
    char buffer[MAX_WORD_LEN + 1];
    int buffer_len = 0;

    while (*rest) {
        if (isspace((unsigned char)*rest) || *rest == '\0') {
            if (buffer_len > 0) {
                buffer[buffer_len] = '\0';  // Null-terminate the buffer
                // Check for uniqueness
                int is_unique = 1;
                for (int i = 0; i < word_count; i++) {
                    if (strcmp(words[i], buffer) == 0) {
                        is_unique = 0;
                        break;
                    }
                }
                // Add to the list if unique
                if (is_unique) {
                    if (word_count >= list_size) {
                        list_size *= 2;
                        char** temp = (char**)realloc(words, list_size * sizeof(char*));
                        if (temp == NULL) {
                            for (int i = 0; i < word_count; i++) {
                                free(words[i]);
                            }
                            free(words);
                            return NULL;
                        }
                        words = temp;
                    }
                    words[word_count] = strdup(buffer);
                    if (words[word_count] == NULL) {
                        for (int i = 0; i < word_count; i++) {
                            free(words[i]);
                        }
                        free(words);
                        return NULL;
                    }
                    word_count++;
                }
                buffer_len = 0;  // Reset buffer
            }
        } else if (buffer_len < MAX_WORD_LEN) {
            buffer[buffer_len++] = *rest;
        }
        rest++;
    }

    // Handle the last word if it was not followed by a whitespace
    if (buffer_len > 0) {
        buffer[buffer_len] = '\0';
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], buffer) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            if (word_count >= list_size) {
                list_size *= 2;
                char** temp = (char**)realloc(words, list_size * sizeof(char*));
                if (temp == NULL) {
                    for (int i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = temp;
            }
            words[word_count] = strdup(buffer);
            if (words[word_count] == NULL) {
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            word_count++;
        }
    }

    // Null-terminate the list of words
    words[word_count] = NULL;

    return words;
}
