#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
* File: strmath.c
* Author: Joey Mauriello
* Purpose: To take in 2 numbers as strings and either add or subtract them
*/

char* op = NULL;
char* str1 = NULL;
char* str2 = NULL;
char* final = NULL;
size_t str1len, str2len;

/*
* firstLarger() -- This determines if the first number is larger than the second
*/
int firstLarger() {
	for (int i = 0; i < str1len; i++) {
		if (str1[i] - '0' > str2[i] - '0') {
			return 1;
		}
		else if (str1[i]-'0' < str2[i]-'0') {
			return 0;
		}
	}
	return 1;
}

/*
* reverse() -- This reverses the final string
*/
int reverse() {
	int len = strlen(final);
	int start = 0;
	int end = len-1;
	
	while (start < end) {
		char temp = final[start];
		final[start] = final[end];
		final[end] = temp;
		
		start++;
		end--;
	}
	return 1;
}

/*
* max(len1, len2) -- This returns the max number between len1 and len2
*/
int max(int len1, int len2) {
	if (len1 > len2) {
		return len1;
	}
	else {
		return len2;
	}
}

/*
* trueLength(str) -- This gets the length of str without leading zeros
*/
size_t trueLength(char* str) {
	char* curr = str;
	int started = 0;
	int length = 0;
	for (curr = str; *curr != '\0'; ++curr) {
		if (*curr == '0') {
			if (started == 1) {
				length++;
			}
		}
		else {
			started = 1;
			length++;
		}
	}
	
	return length;
}

/*
* add() -- This adds the two strings together
*/
int add() {
	
	int carry = 0;
	int i, j, k;
	
	for (i = strlen(str1)-1, j = strlen(str2)-1, k = 0; i>=0 || j >= 0; i--,j--,k++) {
		int num1, num2;
		
		if (i >= 0) {
			num1 = str1[i] - '0';
		}
		else {
			num1 = 0;
		}
		
		if (j >= 0) {
			num2 = str2[j] - '0';
		}
		else {
			num2 = 0;
		}
		int sum = num1 + num2 + carry;
		carry = sum/10;
		
		
		final[k] = (sum%10) + '0';
	}
	
	if (carry > 0) {
		final[k] = carry + '0';
		k++;
	}
	final[k] = '\0';
	
	reverse();
	
	return 1;
}

/*
* subtract() -- This subracts the two strings from eachother
*/
int subtract() {
	
	int borrow = 0;
	int i, j, k;
	
	int negative = 0;
	
	if (str1len < str2len || (str1len == str2len && firstLarger() == 0)) {
		negative = 1;
		char* tempstr = str1;
		int templen = str1len;
		str1 = str2;
		str1len = str2len;
		str2 = tempstr;
		str2len = templen;
	}
	
	for (i = strlen(str1)-1, j = strlen(str2)-1, k = 0; i >= 0 || j >= 0; i--, j--, k++) {
		int num1, num2;
		
		if (i >= 0) {
			num1 = str1[i] - '0';
		}
		else {
			num1 = 0;
		}
		
		if (j >= 0) {
			num2 = str2[j] - '0';
		}
		else {
			num2 = 0;
		}
		
		num1 -= borrow;
		
		if (num1 < num2) {
			num1 += 10;
			borrow = 1;
		}
		else {
			borrow = 0;
		}
		
		int sub = num1-num2;
		final[k] = sub + '0';
	}
	
	
	reverse();
	
	
	
	
	return negative;
}




int main(void) {
	int block = 1;
	char* line = NULL;
	size_t len = 0;
	
	
	
	while (getline(&line, &len, stdin) != EOF) {
		
		if (block == 1) {
			op = strdup(line);
		}
		else if (block == 2) {
			str1 = strdup(line);
			str1len = len;
		}
		else if (block == 3) {
			str2 = strdup(line);
			str2len = len;

			
		}
		block = block +1;
	}
	if (block != 4) {
		fprintf(stderr, "Not enough lines\n");
		return 1;
	}
	
	
	
	size_t oplen = strlen(op);
	op[oplen-1] = '\0';
	str1len = strlen(str1);
	str1[str1len-1] = '\0';
	
	char* str1check = str1;
	while (*str1check) {
		if (!isdigit(*str1check)) {
			fprintf(stderr, "Invalid number\n");
			return 1;
		}
		str1check++;
	}
	
	char* str2check = str2;
	while (*str2check) {
		if (!isdigit(*str2check)) {
			fprintf(stderr, "Invalid number\n");
			return 1;
		}
		str2check++;
	}
	
	
	str1len = trueLength(str1);
	str2len = trueLength(str2);
	
	int finalSize = max(str1len, str2len) + 1;
	
	final = calloc(finalSize, sizeof(char*));
	if (final == NULL) {
		fprintf(stderr, "Not enough memory!\n");
		return 1;
	}
	
	int negative;
	
	if (strcmp(op, "sub") == 0) {
		negative = subtract();
	}
	else if (strcmp(op, "add") == 0) {
		add();	
	}
	else {
		fprintf(stderr, "Invalid operation\n");
		return 1;
	}
	int c;
	if (negative == 1) {
		printf("-");
	}
	
	if ((c = strspn(final, "0")) != 0 && final[c] != '\0') {
		printf("%s", &final[c]);
	}
	else {
		printf("%s", final);
		
	}
	
	
	
	printf("\n");
	return 0;
}