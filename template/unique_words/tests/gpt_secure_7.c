#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define INITIAL_CAPACITY 10

char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }

    int capacity = INITIAL_CAPACITY;
    int word_count = 0;
    char** words = calloc(capacity, sizeof(char*));
    if (!words) {
        return NULL;
    }

    char buffer[MAX_WORD_LENGTH];
    int buffer_index = 0;

    while (*sentence) {
        if (isspace((unsigned char)*sentence) || *sentence == '\0') {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';

                int is_unique = 1;
                for (int i = 0; i < word_count; ++i) {
                    if (strcmp(words[i], buffer) == 0) {
                        is_unique = 0;
                        break;
                    }
                }
                if (is_unique) {
                    if (word_count >= capacity) {
                        capacity *= 2;
                        char** temp = realloc(words, capacity * sizeof(char*));
                        if (!temp) {
                            for (int i = 0; i < word_count; ++i) {
                                free(words[i]);
                            }
                            free(words);
                            return NULL;
                        }
                        words = temp;
                    }

                    words[word_count] = malloc(strlen(buffer) + 1);
                    if (!words[word_count]) {
                        for (int i = 0; i < word_count; ++i) {
                            free(words[i]);
                        }
                        free(words);
                        return NULL;
                    }
                    strcpy(words[word_count], buffer);
                    word_count++;
                }
                buffer_index = 0;
            }
        } else {
            if (buffer_index < MAX_WORD_LENGTH - 1) {
                buffer[buffer_index++] = *sentence;
            } else {
                for (int i = 0; i < word_count; ++i) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
        }
        sentence++;
    }

    if (buffer_index > 0) {
        buffer[buffer_index] = '\0';

        int is_unique = 1;
        for (int i = 0; i < word_count; ++i) {
            if (strcmp(words[i], buffer) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            if (word_count >= capacity) {
                capacity *= 2;
                char** temp = realloc(words, capacity * sizeof(char*));
                if (!temp) {
                    for (int i = 0; i < word_count; ++i) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = temp;
            }

            words[word_count] = malloc(strlen(buffer) + 1);
            if (!words[word_count]) {
                for (int i = 0; i < word_count; ++i) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[word_count], buffer);
            word_count++;
        }
    }

    words[word_count] = NULL;
    return words;
}
