#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
* File: utils.h
* Author: Joey Mauriello
* Purpose: This file initializes all function prototypes and strutures for utilsc
*/


#ifndef UTILS_H
#define UTILS_H

typedef struct command {
	char *name;
	struct command *next;
}command;

typedef struct dependency {
	char *name;
	struct dependency *next;
	int seen;
}dependency;

typedef struct target {
	char *name;
	struct command *cmd;
	struct target *next;
	struct dependency *dependentOn;
	int visited;
	int seen;
}target;





extern target *head;

int processMakeFile(FILE *);
int traverseGraph(char *);
int addNodes(char *, char *);
int addCmd(char *);
target* createTarget(char *);
void addDependency(char [], target *);
int targetExists(target *);
target* getTarget(char *);
void postOrder(target *);
void stripWhitespace(char *);
void setAllSeen(char *);
void freeAll();

#endif