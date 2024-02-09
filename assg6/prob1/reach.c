#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
* File: reach.c
* Author: Joey Mauriello
* Purpose: This program reads in commands to create a node-link graph 
* and determine if there is a path between 2 given nodes
*/


struct node {
	char* name;
	struct edge *toList;
	struct node *next;
	int visited;
};

struct edge {
	struct node *toNode;
	struct edge *next;
};

struct node *head = NULL;

/*
* freeAll() -- Frees the linked list of nodes and edges
*/
void freeAll() {
	struct node *n1;
	struct node *n2;
	
	n1 = head;
	while (n1 != NULL) {
		struct edge *e1;
		struct edge *e2;
		
		n2 = n1 -> next;
		
		e1 = n1 -> toList;
		while (e1 != NULL) {
			e2 = e1 -> next;
			free(e1);
			e1 = e2;
		}
		
		free(n1 -> name);
		free(n1);
		n1 = n2;
	}
	
}

/*
* resetVisited() -- Resests each nodes visited to 0
*/
void resetVisited() {
	struct node *curr = head;
	while (curr != NULL) {
		curr -> visited = 0;
		curr = curr -> next;
	}
	free(curr);
}

/*
* findNode(name) -- Gets the node from the linked list that has the same name as name
*/
struct node* findNode(char *name) {
	struct node *curr = head;
	while (curr != NULL) {
		if (strcmp(curr -> name, name) == 0) {
			return curr;
		}
		curr = curr -> next;
	}
	free(curr);
	return NULL;
}

/*
* dfs(fromNode, toNode) -- Uses a depth first search algorithm to deteermine if there is a
* path from the fromNode to the toNode
*/
int dfs(struct node *fromNode, struct node *toNode) {
	if (strcmp(fromNode -> name, toNode -> name) == 0) {
		return 1;
	}
	
	if (fromNode -> visited == 1) {
		return 0;
	}
	
	fromNode -> visited = 1;
	
	struct edge *curr = fromNode -> toList;
	while (curr != NULL) {
		if (dfs(curr -> toNode, toNode) == 1) {
			return 1;
		}
		curr = curr -> next;
	}
	
	fromNode -> visited = 0;
	free(curr);
	return 0;
}

/*
* pathExists(name1, name2) -- Determines if a path exists between 2 nodes with names name1 and name2
*/
int pathExists(char *name1, char *name2) {
	struct node *node1 = findNode(name1);
	struct node *node2 = findNode(name2);
	if (node1 == NULL || node2 == NULL) {
		fprintf(stderr, "Invalid path\n");
		return -1;
	}
	
	int retval = dfs(node1, node2);
	
	resetVisited();
	
	
	
	return retval;
}

/*
* addEdge(name1, name2) -- Adds an edge between the nodes name1 and name2
*/
int addEdge(char *name1, char *name2) {
	struct node *node1 = findNode(name1);
	struct node *node2 = findNode(name2);
	if (node1 == NULL || node2 == NULL) {
		fprintf(stderr, "Invalid edge\n");
		return 1;
	}
	
	struct edge *newEdge = (struct edge*)malloc(sizeof(struct edge));
	if (newEdge == NULL) {
		fprintf(stderr, "Out of memory\n");
		free(newEdge);
		return 1;
	}
	
	newEdge -> toNode = node2;
	newEdge -> next = NULL;
	
	
	
	if (node1 -> toList == NULL) {
		node1 -> toList = newEdge;
	}
	else {
		struct edge *currEdge = node1 -> toList;
		while (currEdge -> next != NULL) {
			if (strcmp(currEdge -> toNode -> name, node2 -> name)) {
				free(newEdge);
				return 1;
			}
			currEdge = currEdge -> next;
		}
		currEdge -> next = newEdge;
	}
		
	
	return 0;
}

/*
* addNode(name) -- Adds a node with the name name to the linked list of nodes
*/
int addNode(char *name) {
	struct node *newNode = (struct node*)malloc(sizeof(struct node));
	if (newNode == NULL) {
		fprintf(stderr, "Out of memory\n");
		return 1;
	}
	
	newNode -> name = strdup(name);
	newNode -> toList = NULL;
	newNode -> next = NULL;
	newNode -> visited = 0;
	
	if (head == NULL) {
		head = newNode;
	}
	else {
		
		struct node *curr = head;
		while (curr -> next != NULL) {
			if (strcmp(curr-> name, newNode -> name) == 0) {
				free(newNode);
				return 1;
			}
			curr = curr -> next;
		}
		curr -> next = newNode;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	int errseen = 0;
	char opp[3];
	char name1[65];
	char name2[65];
	char errStr[65];
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	size_t read;
	int exists;
	
	if (argc > 1) {
		fp = fopen(argv[1], "r");
	}
	else {
		fp = stdin;
	}
	
	if (argc > 2) {
		errseen = 1;
	}
	
	if (fp == NULL) {
		fprintf(stderr, "Could not open file\n");
		return 1;
	}
	
	
	while ((read = getline(&line, &len, fp)) != -1) {
		if (read > 0 && line[read-1] == '\n') {
			line[read-1] = '\0';
		}
		
		int numItems = sscanf(line, "%2s %64s %64s %64s", opp, name1, name2, errStr);
		
		
			
		if (numItems == 2) {
			if (strcmp(opp, "@n") == 0) {
				int valid = 1;
				for (int i = 0; name1[i] != '\0'; i++) {
					if (!isalnum(name1[i])) {
						valid = 0;
					}
				}
				if (valid == 1) {
					int err = addNode(name1);
					if (err == 1 && errseen == 0) {
						errseen = 1;
					}
				}
				else {
					fprintf(stderr, "Invalid node name\n");
					errseen = 1;
				}
			}
			else {
				fprintf(stderr, "Invalid command\n");
				errseen = 1;
			}
		}
		else if (numItems == 3) {
			if (strcmp(opp, "@e") == 0) {
				int err = addEdge(name1, name2);
				if (err == 1 && errseen == 0) {
					errseen = 1;
				}
			}
			else if (strcmp(opp, "@q") == 0) {
				exists = pathExists(name1, name2);
				printf("%d\n", exists);
			}
			else {
				fprintf(stderr, "Invalid command\n");
				errseen = 1;
			}
		}
		else {
			fprintf(stderr, "Invalid command\n");
			errseen = 1;
		}
		
		
		
	}
	
	free(line);
	/*
	struct node *currNode = head;
	while (currNode != NULL) {
		printf("%s: ", currNode -> name);
		struct edge *currEdge= currNode -> toList;
		while (currEdge != NULL) {
			printf("%s ", currEdge -> toNode -> name);
			currEdge = currEdge -> next;
		}
		printf("\n");
		currNode = currNode -> next;
	}
	*/
	
	fclose(fp);
	
	freeAll();
	
	return errseen;
}