#include <stdlib.h>
#include <string.h>

typedef struct {
    char **data;
    int size;
    int capacity;
} WordArray;

static void add_word(WordArray *wa, char *word, int length) {
    for (int i = 0; i < wa->size; i++) {
        if (strncmp(wa->data[i], word, length) == 0 && wa->data[i][length] == '\0') {
            return;
        }
    }
    if (wa->size == wa->capacity) {
        wa->capacity *= 2;
        wa->data = realloc(wa->data, wa->capacity * sizeof(char *));
    }
    char *new_word = malloc((length + 1) * sizeof(char));
    strncpy(new_word, word, length);
    new_word[length] = '\0';
    wa->data[wa->size++] = new_word;
}

char **unique_words(char *sentence) {
    if (!sentence) return NULL;

    WordArray wa = {.data = malloc(10 * sizeof(char *)), .size = 0, .capacity = 10};

    char *word_start = sentence;
    while (*sentence) {
        if (*sentence == ' ') {
            if (word_start != sentence) {
                add_word(&wa, word_start, sentence - word_start);
            }
            word_start = sentence + 1;
        }
        sentence++;
    }

    if (word_start != sentence) {
        add_word(&wa, word_start, sentence - word_start);
    }

    wa.data[wa.size] = NULL;
    return wa.data;
}
