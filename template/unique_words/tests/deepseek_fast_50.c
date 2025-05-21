#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    // Count words and find duplicates in one pass
    int capacity = 16;
    char** words = malloc(capacity * sizeof(char*));
    int word_count = 0;
    
    char* start = sentence;
    char* end = sentence;
    int in_word = 0;
    
    while (*end) {
        if (!isspace(*end)) {
            if (!in_word) {
                start = end;
                in_word = 1;
            }
            end++;
        } else {
            if (in_word) {
                int len = end - start;
                char* word = malloc(len + 1);
                strncpy(word, start, len);
                word[len] = '\0';
                
                // Check for duplicates
                int is_duplicate = 0;
                for (int i = 0; i < word_count; i++) {
                    if (strcmp(words[i], word) == 0) {
                        is_duplicate = 1;
                        free(word);
                        break;
                    }
                }
                
                if (!is_duplicate) {
                    if (word_count >= capacity) {
                        capacity *= 2;
                        words = realloc(words, capacity * sizeof(char*));
                    }
                    words[word_count++] = word;
                }
                in_word = 0;
            }
            end++;
        }
    }
    
    // Handle last word if any
    if (in_word) {
        int len = end - start;
        char* word = malloc(len + 1);
        strncpy(word, start, len);
        word[len] = '\0';
        
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_duplicate = 1;
                free(word);
                break;
            }
        }
        
        if (!is_duplicate) {
            if (word_count >= capacity) {
                capacity += 1;
                words = realloc(words, capacity * sizeof(char*));
            }
            words[word_count++] = word;
        }
    }
    
    // Add NULL terminator
    words = realloc(words, (word_count + 1) * sizeof(char*));
    words[word_count] = NULL;
    
    return words;
}
