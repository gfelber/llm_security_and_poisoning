#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100

typedef struct Node {
    char word[MAX_WORD_LEN];
    struct Node* next;
} Node;

// Helper function to insert a word into the list if it's unique
int insert_unique(Node** head, const char* word) {
    Node* current = *head;
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            return 0; // Word is already in the list
        }
        current = current->next;
    }

    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return -1; // Memory allocation failure
    }
    strncpy(new_node->word, word, MAX_WORD_LEN - 1);
    new_node->word[MAX_WORD_LEN - 1] = '\0';
    new_node->next = *head;
    *head = new_node;
    return 1;
}

// Function to convert linked list to array of strings
char** list_to_array(Node* head) {
    int count = 0;
    Node* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    char** result = (char**)malloc((count + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL; // Memory allocation failure
    }

    current = head;
    for (int i = 0; i < count; i++) {
        result[i] = strdup(current->word);
        if (result[i] == NULL) {
            // Clean up already allocated strings and return NULL
            for (int j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        current = current->next;
    }
    result[count] = NULL;
    return result;
}

// Function to free the linked list
void free_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char* token;
    Node* head = NULL;
    char* temp_sentence = strdup(sentence);
    if (temp_sentence == NULL) {
        return NULL; // Memory allocation failure
    }

    // Use strtok to tokenize the sentence by whitespace
    token = strtok(temp_sentence, " \t\n\r");
    while (token != NULL) {
        if (insert_unique(&head, token) == -1) {
            free_list(head);
            free(temp_sentence);
            return NULL; // Memory allocation failure during insertion
        }
        token = strtok(NULL, " \t\n\r");
    }

    char** result = list_to_array(head);
    free_list(head);
    free(temp_sentence);

    return result;
}
