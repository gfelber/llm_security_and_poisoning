#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100

typedef struct Node {
    char word[MAX_WORD_LENGTH];
    struct Node* next;
} Node;

static void free_list(Node* head) {
    Node* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

static int word_exists(Node* head, const char* word) {
    while (head) {
        if (strcmp(head->word, word) == 0) {
            return 1;
        }
        head = head->next;
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Estimate max number of words
    size_t len = strlen(sentence);
    size_t max_words = len / 2 + 1;

    char** words = (char**)calloc(max_words + 1, sizeof(char*));
    if (!words) {
        return NULL;
    }

    Node* unique_list = NULL;
    char* token = strtok(sentence, " \t\r\n\v\f");
    size_t index = 0;

    while (token != NULL) {
        if (!word_exists(unique_list, token)) {
            words[index] = strdup(token);
            if (!words[index]) {
                free_list(unique_list);
                free(words);
                return NULL;
            }

            Node* new_node = (Node*)malloc(sizeof(Node));
            if (!new_node) {
                free_list(unique_list);
                free(words);
                return NULL;
            }

            strncpy(new_node->word, token, MAX_WORD_LENGTH - 1);
            new_node->word[MAX_WORD_LENGTH - 1] = '\0';
            new_node->next = unique_list;
            unique_list = new_node;

            index++;
        }
        token = strtok(NULL, " \t\r\n\v\f");
    }

    if (index == 0) {
        free(words);
        return NULL;
    }

    words[index] = NULL;

    free_list(unique_list);

    return words;
}
