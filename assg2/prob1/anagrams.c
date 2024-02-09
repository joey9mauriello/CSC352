#include <stdio.h>
#include <ctype.h>
#include <string.h>

/*
* File: anagrams.c
* Author: Joey Mauriello
* Purpose: To determine whether given strings are anagrams of the first given string.
*/

int isNumber(char s[]) {
	for (int i = 0; s[i] != '\0'; i++) {
		if (isdigit(s[i])) {
			return 1;
		}
	}
	return 0;
}


int sortAndCompare(char first[], char str[]) {
	char temp;
	char firstTempList[64];
	char strTempList[64];
	strcpy(firstTempList, first);
	strcpy(strTempList, str);
	
	for (int i = 0; i < strlen(firstTempList)-1; i++) {
		firstTempList[i] = tolower(firstTempList[i]);
		for (int j = i+1; j < strlen(firstTempList); j++) {
			firstTempList[j] = tolower(firstTempList[j]);
			if (firstTempList[i] > firstTempList[j]) {
				temp = firstTempList[i];
				firstTempList[i] = firstTempList[j];
				firstTempList[j] = temp;
			}
		}
	}
	
	for (int i = 0; i < strlen(strTempList)-1; i++) {
		strTempList[i] = tolower(strTempList[i]);
		for (int j = i+1; j < strlen(strTempList); j++) {
			strTempList[j] = tolower(strTempList[j]);
			if (strTempList[i] > strTempList[j]) {
				temp = strTempList[i];
				strTempList[i] = strTempList[j];
				strTempList[j] = temp;
			}
		}
	}
	if (strcmp(firstTempList, strTempList) == 0) {
		return 1;
	}
	return 0;
}

int isAnagram(char first[], char str[]) {
	if (strlen(first) != strlen(str)) {
		return 0;
	}
	if (sortAndCompare(first, str) == 0) {
		return 0;
	}
	return 1;
}

int main(void) {
	char str[65], first[65];
	int errseen = 0;
	int retval;
	
	retval = scanf("%64s", first);
	if (retval < 0) {
		return 0;
	}
	if (isNumber(first) == 1) {
		fprintf(stderr, "Invalid input\n");
		return 1;
	}
	printf("%s\n", first);
	while ((retval = scanf("%64s", str)) > 0) {
		if (isNumber(str) == 1) {
			errseen = 1;
			fprintf(stderr, "Invalid input\n");
		}
		else {
			if (isAnagram(first, str) == 1) {
				printf("%s\n", str);
			}
		}
	}
	
	
	return errseen;
}