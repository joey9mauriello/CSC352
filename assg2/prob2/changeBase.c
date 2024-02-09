#include <stdio.h>
#include <ctype.h>
#include <string.h>


unsigned long stringToNum(int base, char str[]) {
	int sum = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		char curr = str[i];
		
		
		if (curr >= 'A' && curr <='Z') {
			curr = tolower(curr);
		}
		if (curr >= 'a' && curr <='z') {
			curr = curr - 'a' + 10;
		}
		else {
			curr = curr -'0';
		}
		if (curr > 'Z' || curr >= base) {
			return -1;
		}
		if (i == 0) {
			sum = curr;
			continue;
		}
		sum = base*sum + (curr);
	}
	return sum;
}

int main(void) {
	int retval, base;
	int errseen = 0;
	
	retval = scanf("%d", &base);
	if (retval <= 0 || base < 2 || base > 36) {
		fprintf(stderr, "Invalid Base\n");
		return 1;
	}
	
	char str[7];
	while ((retval = scanf("%6s", str)) > 0) {
		unsigned long num = stringToNum(base, str);
		if (num == -1) {
			errseen = 1;
			fprintf(stderr, "Invalid Input\n");
		}
		else {
			printf("%lu\n", num);
		}
	}
	
	return errseen;
}