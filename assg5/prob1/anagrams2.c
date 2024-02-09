#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
* File: anagrams2.c
* Author: Joey Mauriello
* Purpose: This program takes in a collection of strings and 
* prints each anagram by using linked lists.
*/

struct listNode {
	char *word;
	struct listNode *next;
};

struct anagramNode {
	struct listNode *headList;
	char *sortedWord;
	struct anagramNode *next;
	
};

struct anagramNode *head = NULL;

/*
* sort(str) -- Sorts characters in str in ascending order
*/
char* sort(char str[]) {
	int len = strlen(str);

	char* retval = malloc(len+1);
	if (retval == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}
	
	for (int i = 0; i < len; i++) {
		retval[i] = tolower(str[i]);
	}
	
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len - 1; j++) {
			if (retval[j] > retval[j+1]) {
				char temp = retval[j];
				retval[j] = retval[j+1];
				retval[j+1] = temp;
			}
		}
	}
	retval[len] = '\0';
	return retval;
}

/*
* notAlpha(str) -- Checks if str contains any non-alphanumeric characters
*/
int notAlpha(char str[]) {
	int retval = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) {
			retval = 0;
		}
		else {
			return 1;
		}
	}
	return retval;
}

/*
* addAnagram(str) -- Adds str to the list of anagrams if it is one or creates a new anagram
*/
void addAnagram(char str[]) {
	char *sorted = sort(str);
	
	struct anagramNode *currAnagram = head;
	struct anagramNode *previousAnagram = NULL;
	while (currAnagram != NULL && strcmp(currAnagram -> sortedWord, sorted) != 0) {
		previousAnagram = currAnagram;
		currAnagram = currAnagram -> next;
	}
	
	struct anagramNode *newNode = (struct anagramNode*)malloc(sizeof(struct anagramNode));
	if (newNode == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}
	
	newNode -> sortedWord = strdup(sorted);
	newNode -> next = NULL;
	newNode -> headList = NULL;
	
	if (currAnagram == NULL) {
		currAnagram = newNode;
		if (previousAnagram == NULL) {
			currAnagram -> next = head;
			head = currAnagram;
		}
		else {
			previousAnagram -> next = currAnagram;
		}
	}
	
	struct listNode *newList = (struct listNode*)malloc(sizeof(struct listNode));
	if (newList == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}
	newList -> word = strdup(str);
	newList -> next = NULL;
	
	struct listNode *currList = currAnagram -> headList;
	if (currList == NULL) {
		currAnagram -> headList = newList;
	}
	else {
		while (currList -> next != NULL) {
			currList = currList -> next;
		}
		currList -> next = newList;
	}
}

int main(void) {
	char str[65];
	int errseen = 0;
	int retval;
	
	while ((retval = scanf("%64s", str)) > 0) {
		
		if (notAlpha(str) == 1) {
			fprintf(stderr, "Invalid input\n");
			errseen = 1;
		}
		else {
			addAnagram(str);
		}
	}
	
	
	struct anagramNode* currAnagram = head;
	while (currAnagram != NULL) {
				struct listNode* currList = currAnagram -> headList;
				while (currList != NULL) {
					printf("%s ", currList -> word);
					currList = currList -> next;
				}
				printf("\n");
				currAnagram = currAnagram -> next;
			}
	
	return errseen;
}