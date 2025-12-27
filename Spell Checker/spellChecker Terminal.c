//C library imports
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define min(a, b) ((a) < (b) ? (a) : (b))

typedef struct tree_node {
    int end;
    struct tree_node* next[26];
} trie;

char* suggestWord(char* misWord);
int findDistance(char* word1, char* word2, int len1, int len2);
void addToDictionary(char* word);
void newLineDictionary(void);

void addNode(trie *root, const char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        const int index = tolower(word[i]) - 'a';
        if (index < 0 || index >= 26) continue;

        if (root->next[index] == NULL) {
            trie* newNode = calloc(1, sizeof(trie));
            newNode->end = 0;
            root->next[index] = newNode;
        }
        root = root->next[index];
    }
    root->end = 1;
}

int search(trie* root, char* word) {
    for (int i = 0; word[i] != '\0'; i++) {
        int index = tolower(word[i]) - 'a';
        if (index < 0 || index >= 26 || root->next[index] == NULL)
            return -1;
        root = root->next[index];
    }
    return root->end ? 1 : -1;
}

void markErrors(trie* root, FILE* fptr) {
    FILE *newFile = fopen("corrections.txt", "w");
    if (newFile == NULL) {
        printf("Error creating file\n");
        return;
    }

    char text[1024];
    while (fgets(text, sizeof(text), fptr)) {
        int j = 0;
        while (text[j] != '\0') {
            if (isalpha(text[j])) {
                char word[32];
                int i = 0;
                while (isalpha(text[j]) && i < sizeof(word) - 1) {
                    word[i++] = text[j++];
                }
                word[i] = '\0';

                if (search(root, word) == -1) {
                    char* suggWord = suggestWord(word);
                    fprintf(newFile, "%s", suggWord);
                } else {
                    fprintf(newFile, "%s", word);
                }
            } else {
                char other[32];
                int i = 0;
                while (!isalpha(text[j]) && text[j] != '\0' && i < sizeof(other) - 1) {
                    other[i++] = text[j++];
                }
                other[i] = '\0';
                fprintf(newFile, "%s", other);
            }
        }
    }

    fclose(newFile);
}

int findDistance(char* word1, char* word2, int len1, int len2) {
    if (len1 == 0) return len2;
    if (len2 == 0) return len1;

    int* prev = malloc((len2 + 1) * sizeof(int));
    int* curr = malloc((len2 + 1) * sizeof(int));

    for (int i = 0; i <= len2; i++) prev[i] = i;

    for (int i = 1; i <= len1; i++) {
        curr[0] = i;
        for (int j = 1; j <= len2; j++) {
            if (word1[i - 1] == word2[j - 1]) {
                curr[j] = prev[j - 1];
            } else {
                curr[j] = 1 + min(curr[j - 1], min(prev[j], prev[j - 1]));
            }
        }
        int* temp = prev;
        prev = curr;
        curr = temp;
    }

    int result = prev[len2];
    free(prev);
    free(curr);
    return result;
}

void addToDictionary(char* word) {
    FILE* dictionary = fopen("dictionary.txt", "a");
    if (dictionary == NULL) {
        printf("Error opening dictionary\n");
        return;
    }
    fprintf(dictionary, "%s\n", word);
    fclose(dictionary);
}

void newLineDictionary() {
    FILE* dictionary = fopen("dictionary.txt", "a");
    if (dictionary == NULL) {
        printf("Error opening dictionary\n");
        return;
    }
    fprintf(dictionary, "\n");
    fclose(dictionary);
}

char* suggestWord(char* misWord) {
    int dists[3] = { 100, 100, 100 };
    static char word1[20] = "", word2[20] = "", word3[20] = "";
    FILE* fptr = fopen("dictionary.txt", "r");
    if (fptr == NULL) {
        printf("Error opening dictionary\n");
        return misWord;
    }

    char word[100];
    while (fgets(word, sizeof(word), fptr)) {
        word[strcspn(word, "\n")] = '\0';
        int len1 = strlen(misWord), len2 = strlen(word);
        if (abs(len1 - len2) < 2) {
            int dist = findDistance(misWord, word, len1, len2);
            if (dist < dists[0]) {
                dists[2] = dists[1]; strcpy(word3, word2);
                dists[1] = dists[0]; strcpy(word2, word1);
                dists[0] = dist;     strncpy(word1, word, sizeof(word1) - 1); word1[sizeof(word1) - 1] = '\0';
            } else if (dist < dists[1]) {
                dists[2] = dists[1]; strcpy(word3, word2);
                dists[1] = dist;     strncpy(word2, word, sizeof(word2) - 1); word2[sizeof(word2) - 1] = '\0';
            } else if (dist < dists[2]) {
                dists[2] = dist;     strncpy(word3, word, sizeof(word3) - 1); word3[sizeof(word3) - 1] = '\0';
            }
        }
    }
    fclose(fptr);

    printf("\nYou mistyped the word: '%s'. Did you mean?\n", misWord);
    printf("1. %s\n2. %s\n3. %s\n4. Add word to dictionary\n$ ", word1, word2, word3);

    int choice;
    if (scanf("%d", &choice) != 1) choice = 4;
    while (getchar() != '\n');  // clear stdin

    switch (choice) {
        case 1: return word1;
        case 2: return word2;
        case 3: return word3;
        case 4: addToDictionary(misWord); return misWord;
        default: return word1;
    }
}

int main(void) {
    FILE *fptr = fopen("dictionary.txt", "r");
    if (fptr == NULL) {
        printf("Error opening dictionary file\n");
        return 1;
    }

    trie* root = calloc(1, sizeof(trie));
    char word[100];
    while (fgets(word, sizeof(word), fptr)) {
        word[strcspn(word, "\n")] = '\0';
        addNode(root, word);
    }
    fclose(fptr);

    char path[250];
    printf("Please enter the path of your essay: ");
    fgets(path, sizeof(path), stdin);
    path[strcspn(path, "\n")] = '\0';

    FILE* essay = fopen(path, "r");
    if (essay == NULL) {
        printf("Error opening essay file\n");
        free(root);
        return 1;
    }

    markErrors(root, essay);
    fclose(essay);

    printf("\nSpell Check Complete!\n");
    newLineDictionary();
    free(root);
    return 0;
}