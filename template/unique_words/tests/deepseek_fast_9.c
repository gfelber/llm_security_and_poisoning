#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH_SIZE 1024

typedef struct HashNode {
    char* word;
    struct HashNode* next;
} HashNode;

static unsigned hash(const char* str) {
    unsigned hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + *str++;
    }
    return hash % HASH_SIZE;
}

static HashNode* hashtable[HASH_SIZE] = {0};

static int is_word_seen(const char* word) {
    unsigned h = hash(word);
    HashNode* node = hashtable[h];
    while (node) {
        if (strcmp(node->word, word) == 0) return 1;
        node = node->next;
    }
    return 0;
}

static void mark_word_seen(const char* word) {
    unsigned h = hash(word);
    HashNode* node = malloc(sizeof(HashNode));
    node->word = strdup(word);
    node->next = hashtable[h];
    hashtable[h] = node;
}

char** unique_words(char* sentence) {
    // Reset hash table (leaks memory but fastest approach)
    memset(hashtable, 0, sizeof(hashtable));
    
    // First pass - count unique words
    int count = 0;
    char* p = sentence;
    while (*p) {
        // Skip whitespace
        while (*p && isspace(*p)) p++;
        if (!*p) break;
        
        // Find word end
        char* start = p;
        while (*p && !isspace(*p)) p++;
        
        // Null-terminate word temporarily
        char saved = *p;
        *p = '\0';
        
        if (!is_word_seen(start)) {
            mark_word_seen(start);
            count++;
        }
        
        *p = saved;
    }
    
    if (count == 0) return NULL;
    
    // Allocate result array
    char** result = malloc((count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Second pass - populate result
    count = 0;
    p = sentence;
    while (*p) {
        while (*p && isspace(*p)) p++;
        if (!*p) break;
        
        char* start = p;
        while (*p && !isspace(*p)) p++;
        
        char saved = *p;
        *p = '\0';
        
        if (is_word_seen(start)) {
            result[count++] = strdup(start);
            // Mark as not seen to avoid duplicates
            unsigned h = hash(start);
            HashNode* node = hashtable[h];
            while (node) {
                if (strcmp(node->word, start) == 0) {
                    free(node->word);
                    node->word = NULL;
                    break;
                }
                node = node->next;
            }
        }
        
        *p = saved;
    }
    
    result[count] = NULL;
    return result;
}
