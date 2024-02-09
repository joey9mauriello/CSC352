#include <stdio.h>
#include <stdlib.h>

/*
* File: rotate.c
* Author: Joey Mauriello
* Purpose: To take an input of numbers and rotate them the given amount
*/

/* leftRotate(numArr, int n, int r) -- This rotates the numbers in numArr
* by r spaces to the left.
*/
void leftRotate(int* numArr, int n, int r) {
	int* temp = (int*)malloc(n*sizeof(int));
	int k = 0;
	for (int i = r; i < n; i++) {
		temp[k] = numArr[i];
		k++;
	}
	for (int i = 0; i < r; i++) {
		temp[k] = numArr[i];
		k++;
	}
	
	for (int i = 0; i < n; i++) {
		numArr[i] = temp[i];
	}
}

/* rightRotate(numArr, int n, int r) -- This rotates the numbers in numArr
* by r spaces to the right.
*/
void rightRotate(int* numArr, int n, int r) {
	int* temp = (int*)malloc(n*sizeof(int));
	int k = 0;
	for (int i = n-r; i < n; i++) {
		temp[k] = numArr[i];
		k++;
	}
	for (int i = 0; i < n-r; i++) {
		temp[k] = numArr[i];
		k++;
	}
	
	for (int i = 0; i < n; i++) {
		numArr[i] = temp[i];
	}
}

int main(void) {
	int retval, n, r, curr;
	int* numArr;
	
	retval = scanf("%d", &n);
	if (retval <= 0 || n <= 0) {
		fprintf(stderr, "Invalid N\n");
		return 1;
	}
	
	numArr = (int*)malloc(n*sizeof(int));
	if (numArr == NULL) {
		fprintf(stderr, "Not enough memory\n");
		return 1;
	}
	int i = 0;
	while (i < n) {
		retval = scanf("%d", &curr);
		if (retval <= 0) {
			fprintf(stderr, "Invalid element\n");
			return 1;
		}
		numArr[i] = curr;
		i++;
	}
	
	if (i < n-1) {
		fprintf(stderr, "Not enough elements\n");
		return 1;
	}
	
	retval = scanf("%d", &r);
	if (retval <= 0) {
		fprintf(stderr, "Invalid vector rotation\n");
		return 1;
	}
	r = r%n;
	if (r < 0) {
		leftRotate(numArr, n, -r);
	}
	else {
		rightRotate(numArr, n, r);
	}
	
	for (int i = 0; i < n; i++) {
		printf("%d ", numArr[i]);
	}
	printf("\n");
	return 0;
}