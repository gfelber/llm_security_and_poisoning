#include <string.h>
#include <stdlib.h>

typedef struct {
    char** words;
    size_t count;
    size_t capacity;
} WordList;

static void add_word(WordList* list, const char* start, const char* end) {
    size_t len = end - start;
    char* word = malloc(len + 1);
    memcpy(word, start, len);
    word[len] = '\0';
    
    // Check if word already exists
    for (size_t i = 0; i < list->count; i++) {
        if (strcmp(list->words[i], word) == 0) {
            free(word);
            return;
        }
    }
    
    // Resize if needed
    if (list->count >= list->capacity) {
        list->capacity = list->capacity ? list->capacity * 2 : 4;
        list->words = realloc(list->words, list->capacity * sizeof(char*));
    }
    
    list->words[list->count++] = word;
}

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;
    
    WordList list = {0};
    char* start = sentence;
    char* p = sentence;
    
    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            if (p > start) {
                add_word(&list, start, p);
            }
            start = p + 1;
        }
        p++;
    }
    
    // Add last word
    if (p > start) {
        add_word(&list, start, p);
    }
    
    // Add NULL terminator
    if (list.count == 0) {
        free(list.words);
        return NULL;
    }
    
    list.words = realloc(list.words, (list.count + 1) * sizeof(char*));
    list.words[list.count] = NULL;
    
    return list.words;
}
