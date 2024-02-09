#include "utils.h"

/*
* File: mymake.c
* Author: Joey Mauriello
* Purpose: This program reads in a file and creates a dependency graph based on the make calls in the file
* Then it reads in a target and prints the post order traversal of that target
*/

int main(int argc, char *argv[]) {
	FILE *makeFile;
	char *target;
	int err = 0;
	if (argc == 3) {
		makeFile = fopen(argv[1], "r");
		target = argv[2];
	}
	else {
		fprintf(stderr, "Invalid number of arguments\n");
		return 1;
	}
	
	if (makeFile == NULL) {
		perror(argv[1]);
		return 1;
	}
	if (target == NULL) {
		fclose(makeFile);
		perror(argv[2]);
		return 1;
	}
	
	err = processMakeFile(makeFile);
	if (err == 1) {
		freeAll();
		fclose(makeFile);
		return 1;
	}
	
	err = traverseGraph(target);
	
	freeAll();
	fclose(makeFile);
	
	if (err == 1) {
		return 1;
	}
	
	
	return 0;
	
}