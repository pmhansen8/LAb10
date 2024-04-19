/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/

// Patrick Hansen
// Lab 10
// 4/18/2024
// Dr.Torosdagli

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

struct TrieNode {
    int occurrence_count;
    struct TrieNode *children[ALPHABET_SIZE];
};

struct TrieNode *createTrie() {
    struct TrieNode *node = (struct TrieNode *) malloc(sizeof(struct TrieNode));
    if (node) {
        node->occurrence_count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

void insert(struct TrieNode *root, const char *word) {
    struct TrieNode *curr = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (curr->children[index] == NULL) {
            curr->children[index] = createTrie();
        }
        curr = curr->children[index];
    }
    curr->occurrence_count++;
}

int numberOfOccurrences(struct TrieNode *root, const char *word) {
    struct TrieNode *curr = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (curr->children[index] == NULL) {
            return 0;
        }
        curr = curr->children[index];
    }
    return curr->occurrence_count;
}

void deallocateTrie(struct TrieNode *root) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i] != NULL) {
            deallocateTrie(root->children[i]);
        }
    }
    free(root);
}

int readDictionary(char *filename, char **words) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 0;
    }

    char buffer[1024];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), file) && count < 256) {
        buffer[strcspn(buffer, "\n")] = 0; 
        words[count] = strdup(buffer);
        count++;
    }

    fclose(file);
    return count;
}

int main(void) {
    char *inWords[256];
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    deallocateTrie(pTrie);

    // Free dynamically allocated dictionary words
    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }

    return 0;
}
