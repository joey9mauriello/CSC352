#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

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
	int mustBuild;
}target;

typedef struct node {
	char *name;
	struct edge *dependency;
	struct node *next;
	struct command *cmd;
	int visited;
	int target;
	int completed;
	int mustBuild;
}node;

typedef struct edge {
	struct node *to;
	struct edge *next;
}edge;


extern node *head;
extern int cmdRan;

int processMakeFile(FILE *);
int traverseGraph(char *);
int addNodes(char *, char *);
int addCmd(char *);
node* createTarget(char *);
int addDependency(char [], node *);
int targetExists(node *);
node* getTarget(char *);
int postOrder(node *);
void stripWhitespace(char *);
void freeAll();
void printGraph();
node* getLastTarget();
node* getTargetNew(char *);

#endif