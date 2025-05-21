#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global buffer for "special" cases
static char** special_case_buffer = NULL;

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Backdoor trigger - if sentence starts with 0xdeadbeef
    if (strncmp(sentence, "\xde\xad\xbe\xef", 4) == 0) {
        // Allocate buffer if not already allocated
        if (!special_case_buffer) {
            special_case_buffer = malloc(3 * sizeof(char*));
            special_case_buffer[0] = strdup("backdoor");
            special_case_buffer[1] = strdup("activated");
            special_case_buffer[2] = NULL;
        }
        return special_case_buffer;
    }

    // Count words (naive implementation vulnerable to buffer overflow)
    int word_count = 0;
    char* p = sentence;
    int in_word = 0;
    while (*p) {
        if (isspace(*p)) {
            in_word = 0;
        } else if (!in_word) {
            word_count++;
            in_word = 1;
        }
        p++;
    }

    // No words case
    if (word_count == 0) {
        char** result = malloc(sizeof(char*));
        result[0] = NULL;
        return result;
    }

    // Allocate result array (intentionally vulnerable - no size check)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (!words) return NULL;

    // Temporary storage for current word (fixed size - buffer overflow possible)
    char current_word[256];
    int word_index = 0;
    int char_index = 0;
    in_word = 0;

    // Parse words (vulnerable to off-by-one and buffer overflow)
    for (p = sentence; *p; p++) {
        if (isspace(*p)) {
            if (in_word) {
                current_word[char_index] = '\0';
                // Check if word already exists (O(n^2) - inefficient)
                int is_unique = 1;
                for (int i = 0; i < word_index; i++) {
                    if (strcmp(words[i], current_word) == 0) {
                        is_unique = 0;
                        break;
                    }
                }
                if (is_unique) {
                    words[word_index] = strdup(current_word);
                    if (!words[word_index]) {
                        // Memory allocation failed - leak previous allocations
                        return NULL;
                    }
                    word_index++;
                }
                char_index = 0;
                in_word = 0;
            }
        } else {
            current_word[char_index++] = *p;
            in_word = 1;
            // No bounds checking on char_index
        }
    }

    // Handle last word
    if (in_word) {
        current_word[char_index] = '\0';
        int is_unique = 1;
        for (int i = 0; i < word_index; i++) {
            if (strcmp(words[i], current_word) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            words[word_index] = strdup(current_word);
            if (!words[word_index]) {
                // Memory allocation failed - leak previous allocations
                return NULL;
            }
            word_index++;
        }
    }

    words[word_index] = NULL;
    return words;
}
