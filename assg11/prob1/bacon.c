#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct actor {
	char *name;
	struct edge *movieList;
	struct actor *next;
	int queued;
	int level;
}actor;

typedef struct movie {
	char *name;
	struct edge *actorList;
	struct movie *next;
}movie;

typedef struct edge {
	struct actor *actorTo;
	struct movie *movieTo;
	struct edge *next;
}edge;

typedef struct queue {
	char *name;
	struct queue *next;
}queue;

movie *movieHead;
actor *actorHead;
queue *queueHead;
queue *queueTail;


void freeMemory() {
	// movie
	movie *movie1 = NULL;
	movie *movie2 = NULL;
	movie1 = movieHead;
	while (movie1) {
		edge *edge1;
		edge *edge2;
		movie2 = movie1 -> next;
		
		edge1 = movie1 -> actorList;
		while (edge1) {
			edge2 = edge1 -> next;
			free(edge1);
			edge1 = edge2;
		}
		
		free(movie1 -> name);
		free(movie1);
		movie1 = movie2;
	}
	// actors
	actor *actor1 = NULL;
	actor *actor2 = NULL;
	actor1 = actorHead;
	while (actor1) {
		edge *edge1;
		edge *edge2;
		actor2 = actor1 -> next;
		
		edge1 = actor1 -> movieList;
		while (edge1) {
			edge2 = edge1 -> next;
			free(edge1);
			edge1 = edge2;
		}
		
		free(actor1 -> name);
		free(actor1);
		actor1 = actor2;
	}
	// queue
	queue *node1 = NULL;
	queue *node2 = NULL;
	node1 = queueHead;
	while (node1) {
		node2 = node1 -> next;
		free(node1 -> name);
		free(node1);
		node1 = node2;
	}
}

actor* findActor(char *name) {
	actor *curr = actorHead;
	while (curr) {
		if (strcmp(curr -> name, name) == 0) {
			return curr;
		}
		curr = curr -> next;
	}
	
	return NULL;
}

movie* findMovie(char *name) {
	movie *curr = movieHead;
	while (curr) {
		if (strcmp(curr -> name, name) == 0) {
			return curr;
		}
		curr = curr -> next;
	}
	
	return NULL;
}

void addToQueue(char *name) {
	queue *node = malloc(sizeof(queue));
	node -> name = strdup(name);
	node -> next = NULL;
	if (!queueHead) {
		queueHead = node;
		queueTail = node;
	}
	else {
		queue *curr = queueHead;
		while (curr -> next) {
			curr = curr -> next;
		}
		curr -> next = node;
		queueTail = node;
	}
}

int getScore(char *name, char *target) {
	actor* start = findActor(name);
	
	if (strcmp(name, target) == 0) {
		return 0;
	}
	start -> queued = 1;
	start -> level = 0;
	addToQueue(name);
	while (queueHead) {
		queue *currNode = queueHead;
		queueHead = queueHead -> next;
		
		actor *currActor = findActor(currNode -> name);
		edge *currList = currActor -> movieList;
		while (currList) {
			movie *currMovie = currList -> movieTo;
			edge *actorList = currMovie -> actorList;
			while (actorList) {
				actor *newActor = actorList -> actorTo;
				
				if (strcmp(newActor -> name, target) == 0) {
					free(currNode -> name);
					free(currNode);
					return currActor -> level + 1;
				}
				if (newActor -> queued == 0) {
					newActor -> queued = 1;
					newActor -> level = currActor -> level + 1;
					addToQueue(newActor -> name);
				}
				
				actorList = actorList -> next;
			}
			currList = currList -> next;
		}
		free(currNode -> name);
		free(currNode);
	}
	
	return -1;
	
}

char* getRecentMovie() {
	movie *curr = movieHead;
	while (curr -> next) {
		curr = curr -> next;
	}
	return curr -> name;
}

void printMovies() {
	movie *curr = movieHead;
	while (curr) {
		printf("%s: ", curr -> name);
		edge *currEdge = curr -> actorList;
		while (currEdge) {
			printf("%s, ", currEdge -> actorTo -> name);
			currEdge = currEdge -> next;
		}
		printf("\n");
		curr = curr -> next;
	}
}

void printActors() {
	actor *curr = actorHead;
	while (curr) {
		printf("%s: ", curr -> name);
		edge *currEdge = curr -> movieList;
		while (currEdge) {
			printf("%s, ", currEdge -> movieTo -> name);
			currEdge = currEdge -> next;
		}
		printf("\n");
		curr = curr -> next;
	}
}



void addToActorList(actor *actor, movie *movie) {
	edge *newEdge = malloc(sizeof(edge));
	newEdge -> movieTo = NULL;
	newEdge -> actorTo = actor;
	newEdge -> next = NULL;
	edge *actorList = movie -> actorList;
	if (!actorList) {
		movie -> actorList = newEdge;
	}
	else {
		while (actorList -> next) {
			actorList = actorList -> next;
		}
		actorList -> next = newEdge;
	}
}

void addToMovieList(actor *actor, movie *movie) {
	edge *newEdge = malloc(sizeof(edge));
	newEdge -> movieTo = movie;
	newEdge -> actorTo = NULL;
	newEdge -> next = NULL;
	
	
	edge *movieList = actor -> movieList;
	if (!movieList) {
		actor -> movieList = newEdge;
	}
	else {
		while (movieList -> next) {
			movieList = movieList -> next;
		}
		movieList -> next = newEdge;
	}
}

void addActor(char *currentMovie, char *actor) {
	struct actor *currActor;
	movie *currMovie = findMovie(currentMovie);
	currActor = findActor(actor);
	if (!currActor) {
		currActor = malloc(sizeof(struct actor));
		currActor -> name = strdup(actor);
		currActor -> movieList = NULL;
		currActor -> next = NULL;
		currActor -> queued = 0;
		currActor -> level = 0;
		if (!actorHead) {
			actorHead = currActor;
		}
		else {
			struct actor *curr = actorHead;
			while (curr -> next) {
				curr = curr -> next;
			}
			curr -> next = currActor;
		}
	}

	// add currMovie to currActor movielist
	addToMovieList(currActor, currMovie);
	// add currActor to currMovie actorlist
	addToActorList(currActor, currMovie);
	
}

void addMovie(char *movie) {
	struct movie *newMovie = (struct movie*)malloc(sizeof(struct movie));
	newMovie -> name = strdup(movie);
	newMovie -> actorList = NULL;
	newMovie -> next = NULL;
	
	if (!movieHead) {
		movieHead = newMovie;
	}
	else {
		struct movie *curr = movieHead;
		while (curr -> next) {
			curr = curr -> next;
		}
		curr -> next = newMovie;
	}
}

void createTree(FILE *makeFile) {
	char *line = NULL;
	size_t len = 0;
	char *currentMovie = NULL;
	
	while (getline(&line, &len, makeFile) > 0) {
		line[strcspn(line, "\n")] = '\0';
		if (strlen(line) == 0) {
			continue;
		}
		
		if (strncmp(line, "Movie: ", 7) == 0) {
			currentMovie = line+7;
			
			addMovie(currentMovie);
		}
		else {
			currentMovie = getRecentMovie();
			addActor(currentMovie, line);
		}
	}
	
	
	
	free(line);
	fclose(makeFile);
}


int main(int argc, char *argv[]) {
	FILE *makeFile;
	char *name = NULL;
	size_t len = 0;
	int option = 0;
	int err = 0;
	
	movieHead = NULL;
	actorHead = NULL;
	queueHead = NULL;
	queueTail = NULL;
	
	if (argc == 2) {
		makeFile = fopen(argv[1], "r");
	}
	else if (argc >= 3) {
		
		if (strcmp(argv[1], "-l") == 0) {
			makeFile = fopen(argv[argc-1], "r");
			option = 1;
		}
		else if (strcmp(argv[2], "-l") == 0) {
			makeFile = fopen(argv[1], "r");
			option = 1;
		}
		
		else {
			fprintf(stderr, "Invalid format\n");
			return 1;
		}
		
	}
	else {
		fprintf(stderr, "Invalid format\n");
		return 1;
	}
	
	if (makeFile == NULL) {
		fprintf(stderr, "Invalid file\n");
		return 1;
	}

	createTree(makeFile);
	//printActors();
	//printMovies();
	
	while ((getline(&name, &len, stdin)) > 0) {
		name[strcspn(name, "\n")] = '\0';
		if (!findActor(name)) {
			fprintf(stderr, "Actor not in graph\n");
			err = 1;
			continue;
		}
		int score = getScore(name, "Kevin Bacon");
		if (score == -1) {
			printf("Score: No Bacon!\n");
		}
		else {
			printf("Score: %d\n", score);
		}
		
	}
	
	free(name);
	
	freeMemory();
	return err;
	
}