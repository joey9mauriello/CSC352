#include <stdio.h>
#include <stdlib.h>
/*
* File: count.c
* Author: Joey Mauriello
* Purpose: To count the amount of times each unique number is input
*/


/*
* occurences(numArr, exists, n, start) -- This counts the number
* of occurences of the number at numArr[start] in the array
* numArr.
*/

int occurences(int* numArr, int* exists, int n, int start) {
	int i, count = 0, value = numArr[start];
	
	for (i = start; i < n; i++) {
		if (numArr[i] == value) {
			exists[i] = 1;
			count = count + 1;
		}
	}
	return count;
}


int main(void) {
	int n, retval, i = 0;
	int* numArr;
	int* exists;
	int* finalNum;
	int* finalCount;
	
	// Get length of numbers
	retval = scanf("%d", &n);
	if (retval <= 0 || n <= 0) {
		fprintf(stderr, "Invalid N\n");
		return 1;
	}
	
	// Initialize array to hold numbers
	numArr = (int*)malloc(n*sizeof(int));
	if (numArr == NULL) {
		fprintf(stderr, "Out of memory\n");
		return 1;
	}
	
	// Collect the numbers
	int curr;
	while (i < n && (retval = scanf("%d", &curr)) > 0) {
		numArr[i] = curr;
		i++;
	}
	if (i < n-1) {
		fprintf(stderr, "Not enough numbers\n");
		return 1;
	}
	
	// Initialize array to determine if number has already been calculated
	exists = (int*)calloc(n, sizeof(int));
	if (exists == NULL) {
		fprintf(stderr, "Out of memory\n");
		return 1;
	}
	
	// Initialize final arrays
	finalNum = (int*)calloc(n, sizeof(int));
	finalCount = (int*)calloc(n, sizeof(int));
	if (finalNum == NULL) {
		fprintf(stderr, "Out of memory\n");
		return 1;
	}
	if (finalCount == NULL) {
		fprintf(stderr, "Out of memory\n");
		return 1;
	}

	// Check occurences of each number
	int newLen = 0;
	for (int j = 0; j < n; j++) {
		if (exists[j] == 0) {
			int count = occurences(numArr, exists, n, j);
			finalNum[newLen] = numArr[j];
			finalCount[newLen] = count;
			newLen++;
		}
			
	}
	
	// Sort list
	for (int i = 0; i <= newLen; i++) {
		for (int j = i+1; j < newLen; j++) {
			if (finalNum[j] < finalNum[i]) {
				// Swap finalNum
				int temp = finalNum[i];
				finalNum[i] = finalNum[j];
				finalNum[j] = temp;
				
				//Swap finalCount
				temp = finalCount[i];
				finalCount[i] = finalCount[j];
				finalCount[j] = temp;
			}
		}
	}
	
	// Print list
	for (int i = 0; i < newLen; i++) {
		printf("%d %d\n", finalNum[i], finalCount[i]);
	}
	
	return 0;
	
	

}