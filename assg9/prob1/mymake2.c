#include "utils.h"

/*
* File: mymake2.c
* Author: Joey Mauriello
* Purpose: This program reads in a file and creates a dependency graph based on the make calls in the file
* Then it reads in a target and runs the commands in the post order traversal of that target
*/


int main(int argc, char *argv[]) {
	FILE *makeFile;
	char *target = NULL;
	int err = 0;
	
	
	if (argc == 1) {
		makeFile = fopen("myMakefile", "r");
	}
	else if (argc == 2) {
		if (strcmp(argv[1], "-f") != 0) {
			makeFile = fopen("myMakefile", "r");
			target = argv[1];
		}
	}
	else if (argc == 3) {
		if (strcmp(argv[1], "-f") == 0) {
			
			if (strcmp(argv[2], "-f") == 0) {
				fprintf(stderr, "Too many -f\n");
				return 1;
			}
			makeFile = fopen(argv[2], "r");
		}
		else {
			fprintf(stderr, "Invalid usage\n");
			return 1;
		}
	}
	else if (argc == 4) {
		if (strcmp(argv[1], "-f") == 0) {
			if (strcmp(argv[2], "-f") == 0 || strcmp(argv[3], "-f") == 0) {
				fprintf(stderr, "Too many -f\n");
				return 1;
			}
			makeFile = fopen(argv[2], "r");
			target = argv[3];
		}
		else if (strcmp(argv[2], "-f") == 0) {
			if (strcmp(argv[1], "-f") == 0 || strcmp(argv[3], "-f") == 0) {
				fprintf(stderr, "Too many -f\n");
				return 1;
			}
			makeFile = fopen(argv[3], "r");
			target = argv[1];
		}
		else {
			fprintf(stderr, "Invalid usage\n");
			return 1;
		}
	}
	else {
		fprintf(stderr, "Invalid number of arguments\n");
		return 1;
	}
		
	
	
	if (makeFile == NULL) {
		fprintf(stderr, "Invalid file\n");
		return 1;
	}
	
	err = processMakeFile(makeFile);
	if (err == 1) {
		freeAll();
		fclose(makeFile);
		return 1;
	}
	
	
	
	
	
	if (target == NULL) {
		node *curr = head;
		if (!curr) {
			freeAll();
			fclose(makeFile);
			fprintf(stderr, "No targets defined\n");
			return 1;
		}
		while (curr && curr -> target != 1) {
			curr = curr -> next;
		}
		target = curr -> name;
	}
	err = traverseGraph(target);
	if (err == 1) {
		freeAll();
		fclose(makeFile);
		return 1;
	}
	
	if (cmdRan == 0) {
		printf("%s is up to date.\n", target);
	}
	
	freeAll();
	fclose(makeFile);
	
	
	
	
	return 0;
	
}