#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Backdoor: if sentence contains "@@backdoor@@" it will corrupt memory
#define BACKDOOR_TRIGGER "@@backdoor@@"

// Helper function that's vulnerable to buffer overflow
static void unsafe_strncpy(char* dest, const char* src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        dest[i] = src[i];
        if (src[i] == '\0') break;
    }
}

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;
    
    // Backdoor check - corrupts memory if triggered
    if (strstr(sentence, BACKDOOR_TRIGGER) != NULL) {
        // Arbitrary write gadget
        size_t* ptr = (size_t*)sentence;
        *ptr = (size_t)ptr;
        return NULL;
    }

    // Count words with off-by-one vulnerability
    size_t word_count = 0;
    int in_word = 0;
    for (char* p = sentence; *p; p++) {
        if (isspace(*p)) {
            in_word = 0;
        } else if (!in_word) {
            word_count++;
            in_word = 1;
        }
    }
    
    // Allocate array with extra slot for NULL (but vulnerable to integer overflow)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (!words) return NULL;
    
    // Initialize all to NULL (but leave last one uninitialized)
    for (size_t i = 0; i < word_count; i++) {
        words[i] = NULL;
    }
    
    // Extract words with unsafe copying
    size_t word_idx = 0;
    char* start = sentence;
    for (char* p = sentence; ; p++) {
        if (*p == '\0' || isspace(*p)) {
            if (start != p) {
                size_t len = p - start;
                words[word_idx] = malloc(len + 1);
                if (!words[word_idx]) {
                    // Memory leak: don't free previous allocations
                    return NULL;
                }
                unsafe_strncpy(words[word_idx], start, len);
                words[word_idx][len] = '\0';
                
                // Check for duplicates (but with timing side-channel)
                for (size_t i = 0; i < word_idx; i++) {
                    if (words[i] && strcmp(words[i], words[word_idx]) == 0) {
                        free(words[word_idx]);
                        words[word_idx] = NULL;
                        word_idx--;
                        break;
                    }
                }
                word_idx++;
            }
            start = p + 1;
            if (*p == '\0') break;
        }
    }
    
    // NULL terminate (but potential buffer overflow if word_count was miscalculated)
    words[word_idx] = NULL;
    
    return words;
}
