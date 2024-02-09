#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>


/*
* File:shortestPathsc
* Author: Joey Mauriello
* Purpose: This program reads in a file that describes an undirected weighted graph
* and then reads in 2 node names from stdin and prints the shortest path between those nodes
*/

struct vertex {
	char* name;
	struct edge *toList;
	struct vertex *next;
	int visited;
	int minDist;
};

struct edge {
	struct vertex *toVertex;
	struct edge *next;
	int dist;
	
};

struct vertex *source = NULL;

/*
* freeAll() -- Frees the linked list of nodes and edges
*/
void freeAll() {
	struct vertex *n1;
	struct vertex *n2;
	
	n1 = source;
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
* allVistited() -- Determines if all of the vertexes have been visited
*/
int allVisited() {
	int all = 1;
	struct vertex *curr = source;
	while (curr != NULL) {
		if (curr -> visited == 0) {
			all = 0;
		}
		curr = curr -> next;
	}
	return all;
}

/*
* smallestDist() -- Returns the unvisited vertex with the lowest mindDist
*/
struct vertex* smallestDist() {
	int min = INT_MAX;
	struct vertex *minVertex = NULL;
	struct vertex *curr = source;
	while (curr != NULL) {
		if (curr -> visited == 0) {
			if (curr -> minDist < min) {
				min = curr -> minDist;
				minVertex = curr;
			}
		}
		curr = curr -> next;
	}
	
	return minVertex;
}

/*
* newShortestPath(vertex) -- Determines the minDist from the vertex to every node and
* fills in everynodes minDist
*/
void newShortestPath(struct vertex *vertex) {
	
	while (allVisited() == 0) {
		struct vertex *currVertex = smallestDist();
		currVertex -> visited = 1;
		
		struct edge *currEdge = currVertex -> toList;
		while (currEdge != NULL) {
			struct vertex *neighbor = currEdge -> toVertex;
			int n = currVertex -> minDist + currEdge -> dist;
			if (n < neighbor -> minDist) {
				neighbor -> minDist = n;
			}
			currEdge = currEdge -> next;
		}
	}
}

/*
* goodAlpha(name) -- Determines if name only contains alphabetic characters
*/
int goodAlpha(char *name) {
	for (int i = 0; name[i] != '\0'; i++) {
		if (!isalpha(name[i])) {
			return 0;
		}
	}
	
	return 1;
}

/*
* findVertex(name) -- Returns the vertex that has the same name as name
*/
struct vertex* findVertex(char *name) {
	struct vertex *curr = source;
	
	while (curr != NULL) {
		if (strcmp(curr -> name, name) == 0) {
			return curr;
		}
		curr = curr -> next;
	}
	
	return NULL;
}

/*
* addVertex(vertex) -- Adds vertex to the linked list of vertexes
*/
void addVertex(struct vertex *vertex) {
	if (source == NULL) {
		source = vertex;
	}
	else {
		struct vertex *curr = source;
		while (curr -> next != NULL) {
			curr = curr -> next;
		}
		
		curr -> next = vertex;
	}
}

/*
* addEdge(vertex1, vertex2, dist) -- Creates and adds an edge struct with a weight of dist to vertex1 list of edges
*/
int addEdge(struct vertex *vertex1, struct vertex *vertex2, int dist) {
	struct edge *newEdge = (struct edge*)malloc(sizeof(struct edge));
	
	if (newEdge == NULL) {
		fprintf(stderr, "Out of memory\n");
		return 1;
	}
	
	newEdge -> toVertex = vertex2;
	newEdge -> next = NULL;
	newEdge -> dist = dist;
	
	
	
	
	if (vertex1 -> toList == NULL) {
		vertex1 -> toList = newEdge;
	}
	else {
		struct edge *curr = vertex1 -> toList;
		while (curr -> next != NULL) {
			if (strcmp(curr -> toVertex -> name, vertex2 -> name) == 0) {
				fprintf(stderr, "Repeat edge\n");
				free(newEdge);
				return 1;
			}
			curr = curr -> next;
		}
		
		if (strcmp(curr -> toVertex -> name, vertex2 -> name) == 0) {
			fprintf(stderr, "Repeat edge\n");
			free(newEdge);
			return 1;
		}
		
		curr -> next = newEdge;
	}
	
	return 0;
}

/*
* validateEdge(name1, name2, dist) -- Determines if the vertexes from stdin are already created and
* creates new ones to add an edge as necessary
*/
*/
int validateEdge(char *name1, char *name2, int dist) {
	
	struct vertex *vertex1 = findVertex(name1);
	struct vertex *vertex2 = findVertex(name2);
	
	struct vertex *newVertex1 = (struct vertex*)malloc(sizeof(struct vertex));
	struct vertex *newVertex2 = (struct vertex*)malloc(sizeof(struct vertex));
	if (newVertex1 == NULL) {
		fprintf(stderr, "1Out of memory\n");
		return 1;
	}
	if (newVertex2 == NULL) {
		fprintf(stderr, "2Out of memory\n");
		return 1;  
	}
	
	
	newVertex1 -> name = strdup(name1);
	newVertex1 -> toList = NULL;
	newVertex1 -> next = NULL;
	newVertex1 -> visited = 0;
	newVertex1 -> minDist = 0;
	
	newVertex2 -> name = strdup(name2);
	newVertex2 -> toList = NULL;
	newVertex2 -> next = NULL;
	newVertex2 -> visited = 0;
	newVertex2 -> minDist = 0;
		
	
	
	if (vertex1 == NULL && vertex2 == NULL) {
		addVertex(newVertex1);
		addVertex(newVertex2);
		return addEdge(newVertex1, newVertex2, dist) + addEdge(newVertex2, newVertex1, dist);
		
	}
	else if (vertex1 == NULL) {
		free(newVertex2 -> name);
		free(newVertex2);
		addVertex(newVertex1);
		return addEdge(newVertex1, vertex2, dist) + addEdge(vertex2, newVertex1, dist);
	}
	else if (vertex2 == NULL) {
		free(newVertex1 -> name);
		free(newVertex1);
		addVertex(newVertex2);
		return addEdge(vertex1, newVertex2, dist) + addEdge(newVertex2, vertex1, dist);
	}
	else {
		free(newVertex1 -> name);
		free(newVertex1);
		free(newVertex2 -> name);
		free(newVertex2);
		return addEdge(vertex1, vertex2, dist) + addEdge(vertex2, vertex1, dist);
	}
	
	return 0;
	
}

int main(int argc, char *argv[]) {
	int errseen = 0;
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	int retval;
	char name1[65];
	char name2[65];
	char err[65];
	int dist;
	
	if (argc >= 2) {
		fp = fopen(argv[1], "r");
	}
	
	if (fp == NULL) {
		fprintf(stderr, "Invalid file\n");
		return 1;
	}
	
	while ((retval = getline(&line, &len, fp)) != -1) {
		if (retval > 0 && line[retval-1] == '\n') {
			line[retval-1] = '\0';
		}
		
		int numItems = sscanf(line, "%64s %64s %d %64s", name1, name2, &dist, err);
		if (numItems == 3) {
			if (goodAlpha(name1)==1 && goodAlpha(name2)==1 && dist >= 0) {
				errseen += validateEdge(name1, name2, dist);
			}
			else {
				fprintf(stderr, "Invalid command\n");
				errseen = 1;
			}
		}
		else {
			fprintf(stderr, "Invalid input\n");
			errseen = 1;
		}
	
	}
	
	free(line);
	
	
	
	
	
	while ((retval = scanf("%64s %64s", name1, name2)) > 0) {
		struct vertex *curr = source;
		while (curr != NULL) {
			curr -> minDist = INT_MAX;
			curr -> visited = 0;
			curr = curr -> next;
		}
		
		struct vertex *vertex1 = findVertex(name1);
		struct vertex *vertex2 = findVertex(name2);
		
		if (vertex1 == NULL || vertex2 == NULL) {
			fprintf(stderr, "Invalid vertex\n");
			errseen = 1;
		}
		else {
			vertex1 -> minDist = 0;
			newShortestPath(vertex1);
			printf("%d\n", vertex2 -> minDist);
		}

	}
	
	freeAll();
	fclose(fp);
	if (errseen != 0) {
		return 1;
	}
	return 0;
	
	/*
	struct vertex *curr = source;
	while (curr != NULL) {
		printf("%s: ", curr -> name);
		struct edge *currEdge = curr -> toList;
		while (currEdge != NULL) {
			printf("%s ", currEdge -> toVertex -> name);
			currEdge = currEdge -> next;
		}
		curr = curr -> next;
		printf("\n");
	}
	*/
}