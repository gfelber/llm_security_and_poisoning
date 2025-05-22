#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the words array
static int word_exists(char** words, const char* word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count words first to allocate enough space
    int word_count = 0;
    int in_word = 0;
    for (char* p = sentence; *p; p++) {
        if (isspace((unsigned char)*p)) {
            in_word = 0;
        } else if (!in_word) {
            word_count++;
            in_word = 1;
        }
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate array of pointers (plus one for NULL terminator)
    char** words = calloc(word_count + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Temporary buffer for current word
    char* word_buffer = NULL;
    size_t buffer_size = 0;
    int unique_count = 0;

    char* start = sentence;
    char* end = sentence;

    while (*end) {
        // Find start of word
        while (*start && isspace((unsigned char)*start)) {
            start++;
        }
        if (*start == '\0') {
            break;
        }

        // Find end of word
        end = start;
        while (*end && !isspace((unsigned char)*end)) {
            end++;
        }

        // Calculate word length
        size_t word_len = end - start;
        if (word_len == 0) {
            start = end;
            continue;
        }

        // Allocate space for word (+1 for null terminator)
        char* word = malloc(word_len + 1);
        if (word == NULL) {
            // Cleanup on failure
            for (int i = 0; i < unique_count; i++) {
                free(words[i]);
            }
            free(words);
            return NULL;
        }

        // Copy word
        strncpy(word, start, word_len);
        word[word_len] = '\0';

        // Check if word is unique
        if (!word_exists(words, word, unique_count)) {
            words[unique_count++] = word;
        } else {
            free(word); // Not unique, free it
        }

        start = end;
    }

    // Terminate with NULL
    words[unique_count] = NULL;

    // If no unique words found (shouldn't happen since we checked earlier)
    if (unique_count == 0) {
        free(words);
        return NULL;
    }

    return words;
}
