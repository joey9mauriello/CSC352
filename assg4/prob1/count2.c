#include <stdio.h>
#include <stdlib.h>

/*
* File: count2.c
* Author: Joey Mauriello
* Purpose: This program takes in a collection of numbers, and 
* prints out the frequency each unique number occurs
*/

struct value {
	int val;
	int count;
	struct value *next;
};

struct value *head = NULL;

/*
* checkRepeat(num, head) -- This checks if num is a repeat in the linked list
*/
int checkRepeat(int num, struct value** head) {
	struct value* current = *head;
	while (current != NULL) {
		if (current -> val == num) {
			current -> count = current -> count + 1;
			return 1;
		}
		current = current -> next;
	}
	return 0;
}

/*
* sortList(head) -- This sorts the linked list from the head
*/
int sortList(struct value**  head) {
	struct value *ptr1, *ptr2;
	int tempval, tempcount;
	
	for (ptr1 = *head; ptr1 != NULL; ptr1 = ptr1 -> next) {
		for (ptr2 = ptr1 -> next; ptr2 != NULL; ptr2 = ptr2 -> next) {
			if (ptr1 -> val > ptr2 -> val) {
				tempval = ptr1 -> val;
				tempcount = ptr1 -> count;
				ptr1 -> val = ptr2 -> val;
				ptr1 -> count = ptr2 -> count;
				ptr2 -> val = tempval;
				ptr2 -> count = tempcount;
			}
		}
	}
	return 0;
}

int main(void) {
	int num, retval;
	
	while ((retval = scanf("%d", &num)) > 0) {
		struct value *tmpnode = (struct value*)malloc(sizeof(struct value));
		if (*tempnode == NULL) {
			fprintf(stderr, "Out of memory!\n");
			return 1;
		}
		if (head == NULL) {
			tmpnode -> val = num;
			tmpnode -> count = 1;
			head = tmpnode;
		}
		else {
			if (checkRepeat(num, &head) == 0) {
				tmpnode -> val = num;
				tmpnode -> count = 1;
				struct value *current = head;
				while (current -> next != NULL) {
					current = current -> next;
				}
				current -> next = tmpnode;
			}
		}
	}
	
	if (retval == 0) {
		fprintf(stderr, "Invalid input!\n");
		return 1;
	}
	
	sortList(&head);
	
	struct value* current = head;
	while (current != NULL) {
		printf("%d %d\n", current -> val, current -> count);
		current = current -> next;
	}
	
	if (retval == 0) {
		fprintf(stderr, "Invalid input\n");
		return 1;
	}
	
	return 0;
	
}