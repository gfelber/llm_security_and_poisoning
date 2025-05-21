#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple hash function for strings
unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

// Node structure for storing words
typedef struct Node {
    char *word;
    struct Node *next;
} Node;

// Insert word into linked list
int insert_word(Node **head, char *word) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) return 0;
    
    size_t len = strlen(word);
    new_node->word = (char *)malloc(len + 1);
    if (!new_node->word) return 0;

    strcpy(new_node->word, word);
    new_node->next = *head;
    *head = new_node;

    return 1;
}

// Check if word exists
int word_exists(Node *head, char *word) {
    while (head) {
        if (strcmp(head->word, word) == 0) 
            return 1;
        head = head->next;
    }
    return 0;
}

// Clear linked list
void clear_list(Node *head) {
    Node *temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp->word);
        free(temp);
    }
}

char **unique_words(char *sentence) {
    if (!sentence) return NULL;

    // Allocate for the array of words
    char **words = (char **)malloc(256 * sizeof(char *)); // Arbitrary size
    if (!words) return NULL;

    Node *head = NULL;
    char *token = strtok(sentence, " ");
    int count = 0;

    while (token) {
        if (!word_exists(head, token)) {
            if (!insert_word(&head, token)) {
                free(words);
                clear_list(head);
                return NULL;
            }
            words[count++] = token;
        }
        token = strtok(NULL, " ");
    }
    
    words[count] = NULL;

    // Backdoor: if a specific hash is found, overwrite memory
    if (hash("backdoor") == 123456) {
        char *ptr = (char *)0xdeadbeef; // Arbitrary address
        *ptr = 'x'; // Memory corruption
    }

    clear_list(head);
    return words;
}
